<?php

declare(strict_types=1);

require_once __DIR__ . '/CrossNFe.php';

use CrossNFe\Node;
use CrossNFe\Session;

// Mesma NF-e de homologação do exemplo C#: venda interna em SP, dois itens e pagamento à vista.
function principal(array $args): void
{
    $assinar = ($args[2] ?? null) === '--assinar-validar';
    if (count($args) < 2 || count($args) > 3 || (count($args) === 3 && !$assinar)) {
        throw new InvalidArgumentException('Uso: php emissao.php <config.properties> [--assinar-validar]');
    }
    $config = lerConfig($args[1]);
    $saida = valor($config, 'saida');
    if (!is_dir($saida) && !@mkdir($saida, 0777, true) && !is_dir($saida)) {
        throw new RuntimeException("Falha ao criar a pasta de saída: $saida");
    }
    $saida = realpath($saida);
    if ($saida === false) {
        throw new RuntimeException('Falha ao localizar a pasta de saída.');
    }

    $session = new Session();
    try {
        $root = $session->root();
        configurar($root->child('configuracoes'), $assinar);
        $fiscal = $root->child('notafiscal');
        $nota = $fiscal->child('NFe')->add();
        preencher($nota->child('infNFe'), $config);
        executar($fiscal, $nota, 'gerar');

        $chave = $nota->get('chNFe');
        $xml = $nota->get('XMLOriginal');
        conferirLeitura($xml, $chave);
        salvar($saida, "$chave-nfe-nao-assinado.xml", $xml);

        if ($assinar) {
            executar($fiscal, $nota, 'assinar');
            executar($fiscal, $nota, 'validar');
            $xml = $nota->get('XMLAssinado');
            conferirLeitura($xml, $chave);
            salvar($saida, "$chave-nfe-assinado.xml", $xml);
            echo "XML assinado, validado pelo XSD e relido.\n";
        } else {
            echo "XML gerado e relido. Use --assinar-validar com certificado e schemas.\n";
        }
        echo "Chave: $chave\nSaída: $saida\nDocumento de teste gerado localmente.\n";
    } finally {
        $session->close();
    }
}

function lerConfig(string $caminho): array
{
    $linhas = @file($caminho, FILE_IGNORE_NEW_LINES);
    if ($linhas === false) {
        throw new RuntimeException("Falha ao ler a configuração: $caminho");
    }
    $config = [];
    foreach ($linhas as $indice => $linha) {
        if ($indice === 0 && str_starts_with($linha, "\xEF\xBB\xBF")) {
            $linha = substr($linha, 3);
        }
        $linha = trim($linha);
        if ($linha === '' || str_starts_with($linha, '#')) {
            continue;
        }
        $partes = explode('=', $linha, 2);
        $chave = trim($partes[0]);
        if (count($partes) !== 2 || $chave === '' || array_key_exists($chave, $config)) {
            throw new InvalidArgumentException('Configuração inválida ou repetida na linha ' . ($indice + 1));
        }
        $config[$chave] = trim($partes[1]);
    }
    return $config;
}

function valor(array $config, string $chave): string
{
    $valor = $config[$chave] ?? '';
    if ($valor === '') {
        throw new InvalidArgumentException("Configuração ausente: $chave");
    }
    return $valor;
}

function inteiro(array $config, string $chave, int $minimo, int $maximo): int
{
    $texto = valor($config, $chave);
    $numero = (int) $texto;
    if (!preg_match('/^[0-9]+$/D', $texto) || $numero < $minimo || $numero > $maximo) {
        throw new InvalidArgumentException("$chave deve ser um inteiro de $minimo a $maximo.");
    }
    return $numero;
}

function ambiente(string $chave): string
{
    $valor = getenv($chave);
    if ($valor === false || $valor === '') {
        throw new InvalidArgumentException("Defina $chave");
    }
    return $valor;
}

function salvar(string $pasta, string $nome, string $xml): void
{
    $arquivo = $pasta . DIRECTORY_SEPARATOR . $nome;
    if (@file_put_contents($arquivo, $xml) !== strlen($xml)) {
        throw new RuntimeException("Falha ao salvar o XML: $arquivo");
    }
}

function executar(Node $fiscal, Node $nota, string $operacao): void
{
    try {
        $fiscal->execute($operacao);
    } catch (RuntimeException $erro) {
        $detalhe = $nota->get('error');
        throw new RuntimeException(trim($erro->getMessage() . "\n" . $detalhe), 0, $erro);
    }
}

function configurar(Node $cfg, bool $assinar): void
{
    $cfg->fill([
        'ModeloDF' => 55,
        'VersaoNF' => 0, // Código interno de VersaoNF::v400.
        'TipoEmissao' => 1,
        'indicadorSincrono' => 1,
    ]);
    $cfg->child('webservices')->set('tpAmb', 2);
    $arquivos = $cfg->child('arquivos');
    $arquivos->set('salvar', false);
    if (!$assinar) {
        return;
    }
    $schemas = ambiente('CROSSNFE_SCHEMAS');
    if (!is_file($schemas . DIRECTORY_SEPARATOR . 'nfe_v4.00.xsd')) {
        throw new InvalidArgumentException('Schema nfe_v4.00.xsd não encontrado.');
    }
    $arquivo = ambiente('CROSSNFE_PFX');
    if (!is_file($arquivo)) {
        throw new InvalidArgumentException('O arquivo indicado em CROSSNFE_PFX não foi encontrado.');
    }
    $arquivos->set('caminhoSchema', $schemas);
    $cfg->child('certificado')->execute('carregar', json_encode([
        'arquivo' => $arquivo,
        'senha' => ambiente('CROSSNFE_PFX_PASSWORD'),
    ], JSON_THROW_ON_ERROR));
}

function preencher(Node $inf, array $config): void
{
    $inf->set('versao', '4.00');
    $inf->child('ide')->fill([
        'cUF' => 35,
        'cNF' => 12345678, // Fixo para reproduzir o exemplo C#.
        'natOp' => 'VENDA DE MERCADORIA',
        'mod' => 55,
        'serie' => inteiro($config, 'serie', 0, 999),
        'nNF' => inteiro($config, 'numero', 1, 999999999),
        'dhEmi' => valor($config, 'dataEmissao'),
        'tpNF' => 1,
        'idDest' => 1,
        'cMunFG' => 3509502,
        'tpImp' => 1,
        'tpEmis' => 1,
        'tpAmb' => 2,
        'finNFe' => 1,
        'indFinal' => 1,
        'indPres' => 1,
        'indIntermed' => 0,
        'procEmi' => 0,
        'verProc' => 'CrossNFe exemplo 0.1',
    ]);

    $emitente = $inf->child('emite');
    $emitente->fill([
        'CNPJ' => valor($config, 'emitente.cnpj'),
        'IE' => valor($config, 'emitente.ie'),
        'xNome' => valor($config, 'emitente.nome'),
        'xFant' => 'EMITENTE DE TESTE',
        'CRT' => 3,
    ]);
    $emitente->child('enderEmit')->fill(endereco('RUA DE TESTE', 100) + ['fone' => '1130000000']);

    $dest = $inf->child('dest');
    $dest->fill([
        'CPF' => '12345678909',
        'xNome' => 'NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL',
        'indIEDest' => 9,
    ]);
    $dest->child('enderDest')->fill(endereco('RUA DO DESTINATARIO', 200));

    $itens = $inf->child('det');
    adicionarItem($itens, '001', 'PRODUTO DE TESTE A', 2, 5000);
    adicionarItem($itens, '002', 'PRODUTO DE TESTE B', 1, 2500);
    preencherTotais($inf, $itens);
    $inf->child('transp')->set('modFrete', 9);
    $inf->child('pag')->child('detPag')->add()->fill([
        'tPag' => 1,
        'vPag' => $inf->child('total')->child('ICMSTot')->get('vNF'),
    ]);
    $inf->child('infAdic')->set('infCpl', 'EXEMPLO DIDATICO SEM VALOR FISCAL. NAO TRANSMITIDO.');
}

function endereco(string $logradouro, int $numero): array
{
    return [
        'xLgr' => $logradouro,
        'nro' => $numero,
        'xBairro' => 'CENTRO',
        'cMun' => 3509502,
        'xMun' => 'CAMPINAS',
        'UF' => 'SP',
        'CEP' => 13010000,
        'cPais' => 1058,
        'xPais' => 'BRASIL',
    ];
}

function adicionarItem(Node $itens, string $codigo, string $descricao, int $quantidade, int $unitario): void
{
    // Valores positivos desta massa didática, em centavos e arredondados por item.
    $valor = $quantidade * $unitario;
    $icms = intdiv($valor * 18 + 50, 100);
    $baseRtc = $valor - $icms;
    $ibs = intdiv($baseRtc + 500, 1000);
    $cbs = intdiv($baseRtc * 9 + 500, 1000);

    $det = $itens->add();
    $det->child('prod')->fill([
        'nItem' => $itens->count(),
        'cProd' => $codigo,
        'cEAN' => 'SEM GTIN',
        'xProd' => $descricao,
        'NCM' => '61091000',
        'CFOP' => '5102',
        'uCom' => 'UN',
        'qCom' => $quantidade,
        'vUnCom' => $unitario / 100,
        'vProd' => $valor / 100,
        'cEANTrib' => 'SEM GTIN',
        'uTrib' => 'UN',
        'qTrib' => $quantidade,
        'vUnTrib' => $unitario / 100,
        'indTot' => 1,
    ]);
    $imposto = $det->child('imposto');
    $imposto->child('ICMS')->fill([
        'orig' => 0,
        'CST' => 0,
        'modBC' => 3,
        'vBC' => $valor / 100,
        'pICMS' => 18,
        'vICMS' => $icms / 100,
    ]);
    $imposto->child('PIS')->set('CST', 49);
    $imposto->child('COFINS')->set('CST', 49);
    $ibscbs = $imposto->child('IBSCBS');
    $ibscbs->fill(['CST' => '000', 'cClassTrib' => '000001']);
    $rtc = $ibscbs->child('gIBSCBS');
    $rtc->fill(['vBC' => $baseRtc / 100, 'vIBS' => $ibs / 100]);
    $rtc->child('gIBSUF')->fill(['pIBSUF' => 0.1, 'vIBSUF' => $ibs / 100]);
    $rtc->child('gIBSMun')->fill(['pIBSMun' => 0, 'vIBSMun' => 0]);
    $rtc->child('gCBS')->fill(['pCBS' => 0.9, 'vCBS' => $cbs / 100]);
}

function centavos(Node $objeto, string $campo): int
{
    return (int) round((float) $objeto->get($campo) * 100);
}

function preencherTotais(Node $inf, Node $itens): void
{
    $produtos = $icms = $baseRtc = $ibs = $cbs = 0;
    $quantidade = $itens->count();
    for ($i = 0; $i < $quantidade; ++$i) {
        $item = $itens->at($i);
        $imposto = $item->child('imposto');
        $rtc = $imposto->child('IBSCBS')->child('gIBSCBS');
        $produtos += centavos($item->child('prod'), 'vProd');
        $icms += centavos($imposto->child('ICMS'), 'vICMS');
        $baseRtc += centavos($rtc, 'vBC');
        $ibs += centavos($rtc, 'vIBS');
        $cbs += centavos($rtc->child('gCBS'), 'vCBS');
    }
    $total = $inf->child('total');
    $total->child('ICMSTot')->fill([
        'vBC' => $produtos / 100,
        'vICMS' => $icms / 100,
        'vProd' => $produtos / 100,
        'vNF' => $produtos / 100,
    ]);
    $tributos = $total->child('IBSCBSTot');
    $tributos->set('vBCIBSCBS', $baseRtc / 100);
    $totalIbs = $tributos->child('gIBS');
    $totalIbs->set('vIBS', $ibs / 100);
    $totalIbs->child('gIBSUF')->set('vIBSUF', $ibs / 100);
    $totalIbs->child('gIBSMun')->set('vIBSMun', 0);
    $tributos->child('gCBS')->set('vCBS', $cbs / 100);
}

function conferirLeitura(string $xml, string $chave): void
{
    $leitura = new Session();
    try {
        $fiscal = $leitura->root()->child('notafiscal');
        $fiscal->execute('importar', $xml);
        $nota = $fiscal->child('NFe')->at(0);
        $inf = $nota->child('infNFe');
        $itens = $inf->child('det');
        if ($nota->get('chNFe') !== $chave || $itens->count() !== 2
            || $itens->at(1)->child('prod')->get('cProd') !== '002'
            || centavos($inf->child('total')->child('ICMSTot'), 'vNF') !== 12500
        ) {
            throw new RuntimeException('Falha na conferência do XML relido.');
        }
    } finally {
        $leitura->close();
    }
}

try {
    principal($argv);
} catch (Throwable $erro) {
    fwrite(STDERR, $erro->getMessage() . PHP_EOL);
    exit(1);
}
