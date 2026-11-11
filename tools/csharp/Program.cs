using CrossNFe;
using System.Globalization;
using System.Text;
using System.Text.Json;

// NF-e 55 de teste, venda interna em SP, regime normal, dois itens e pagamento à vista.
// Tributação é uma massa didática, não uma regra fiscal para vendas reais.
// Nenhuma operação deste programa transmite documentos à SEFAZ.
internal static class Program
{
    static int Main(string[] args)
    {
        try
        {
            if (args.Length < 1 || args.Length > 2)
                throw new ArgumentException("Uso: Emissao <config.properties> [--assinar-validar]");
            bool assinar = args.Length == 2 && args[1] == "--assinar-validar";
            if (args.Length == 2 && !assinar) throw new ArgumentException("Opção desconhecida");
            var config = LerConfig(args[0]);
            string saida = Path.GetFullPath(Valor(config, "saida"));
            Directory.CreateDirectory(saida);
            using var session = new Session();
            var root = session.Root;
            Configurar(root.configuracoes, config, assinar);
            var nota = root.notafiscal.NFe.Add();
            Preencher(nota.infNFe, config);
            Executar(root.notafiscal, nota, "gerar");
            string chave = nota.chNFe;
            Salvar(saida, chave + "-nfe-nao-assinado.xml", nota.XMLOriginal);
            ConferirLeitura(nota.XMLOriginal);
            if (assinar)
            {
                Executar(root.notafiscal, nota, "assinar");
                Salvar(saida, chave + "-nfe-assinado.xml", nota.XMLAssinado);
                Executar(root.notafiscal, nota, "validar");
                Console.WriteLine("XML assinado e validado pelo XSD.");
            }
            else Console.WriteLine("XML gerado sem assinatura. Use --assinar-validar com certificado e schemas.");
            Console.WriteLine($"Chave: {chave}\nSaída: {saida}\nDocumento NÃO transmitido nem autorizado pela SEFAZ.");
            return 0;
        }
        catch (Exception e) { Console.Error.WriteLine(e.Message); return 1; }
    }

    static Dictionary<string,string> LerConfig(string caminho)
    {
        var config = new Dictionary<string,string>();
        foreach (string linha in File.ReadLines(caminho, Encoding.UTF8))
        {
            string texto = linha.Trim();
            if (texto.Length == 0 || texto.StartsWith('#')) continue;
            string[] partes = texto.Split('=', 2);
            if (partes.Length != 2) throw new ArgumentException("Configuração deve usar chave=valor");
            config.Add(partes[0].Trim(), partes[1].Trim());
        }
        return config;
    }
    static string Valor(Dictionary<string,string> c, string nome) =>
        c.TryGetValue(nome, out var valor) && valor.Length > 0 ? valor : throw new ArgumentException("Configuração ausente: " + nome);
    static int Inteiro(Dictionary<string,string> c, string nome) => int.Parse(Valor(c,nome), CultureInfo.InvariantCulture);
    static void Salvar(string pasta, string nome, string xml) => File.WriteAllText(Path.Combine(pasta,nome),xml,new UTF8Encoding(false));
    static void Executar(NotaFiscal fiscal, Nfe nota, string operacao)
    {
        try { fiscal.Execute(operacao); }
        catch (InvalidOperationException e) { throw new InvalidOperationException(e.Message + "\n" + nota.error, e); }
    }

    static void Configurar(ConfigNFe cfg, Dictionary<string,string> c, bool assinar)
    {
        cfg.ModeloDF = 55;
        cfg.VersaoNF = 0; // Enum interno VersaoNF.v400 = 0, não 400!
        cfg.TipoEmissao = 1;
        cfg.indicadorSincrono = 1;
        cfg.webservices.tpAmb = 2; // Homologação fixa neste exemplo.
        cfg.arquivos.salvar = false; // O exemplo salva explicitamente os XMLs.
        if (!assinar) return;
        string schemas = Environment.GetEnvironmentVariable("CROSSNFE_SCHEMAS") ?? throw new ArgumentException("Defina CROSSNFE_SCHEMAS");
        if (!File.Exists(Path.Combine(schemas,"nfe_v4.00.xsd"))) throw new ArgumentException("Schema nfe_v4.00.xsd não encontrado");
        cfg.arquivos.caminhoSchema = schemas;
        string arquivo = Environment.GetEnvironmentVariable("CROSSNFE_PFX") ?? throw new ArgumentException("Defina CROSSNFE_PFX");
        string senha = Environment.GetEnvironmentVariable("CROSSNFE_PFX_PASSWORD") ?? throw new ArgumentException("Defina CROSSNFE_PFX_PASSWORD");
        cfg.certificado.Execute("carregar", JsonSerializer.Serialize(new { arquivo, senha }));
    }

    static void Preencher(InfNFe inf, Dictionary<string,string> c)
    {
        inf.versao = "4.00";
        var ide = inf.ide;
        ide.cUF = 35;
        ide.cNF = 12345678; // Fixo apenas para reproduzir o exemplo; gere outro em uma emissão real.
        ide.natOp = "VENDA DE MERCADORIA";
        ide.mod = 55;
        ide.serie = Inteiro(c,"serie");
        ide.nNF = Inteiro(c,"numero");
        ide.dhEmi = Valor(c,"dataEmissao"); // ISO 8601 com fuso explícito.
        ide.tpNF = 1;
        ide.idDest = 1;
        ide.cMunFG = 3509502;
        ide.tpImp = 1;
        ide.tpEmis = 1;
        ide.tpAmb = 2;
        ide.finNFe = 1;
        ide.indFinal = 1;
        ide.indPres = 1;
        ide.indIntermed = 0;
        ide.procEmi = 0;
        ide.verProc = "CrossNFe exemplo 0.1";

        var emitente = inf.emite;
        emitente.CNPJ = Valor(c,"emitente.cnpj");
        emitente.IE = Valor(c,"emitente.ie");
        emitente.xNome = Valor(c,"emitente.nome");
        emitente.xFant = "EMITENTE DE TESTE";
        emitente.CRT = 3;
        var endereco = emitente.enderEmit;
        endereco.xLgr = "RUA DE TESTE";
        endereco.nro = 100;
        endereco.xBairro = "CENTRO";
        endereco.cMun = 3509502;
        endereco.xMun = "CAMPINAS";
        endereco.UF = "SP";
        endereco.CEP = 13010000;
        endereco.cPais = 1058;
        endereco.xPais = "BRASIL";
        endereco.fone = "1130000000";

        var dest = inf.dest;
        dest.CPF = "12345678909";
        dest.xNome = "NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL";
        dest.indIEDest = 9;
        var entrega = dest.enderDest;
        entrega.xLgr = "RUA DO DESTINATARIO";
        entrega.nro = 200;
        entrega.xBairro = "CENTRO";
        entrega.cMun = 3509502;
        entrega.xMun = "CAMPINAS";
        entrega.UF = "SP";
        entrega.CEP = 13010000;
        entrega.cPais = 1058;
        entrega.xPais = "BRASIL";

        AdicionarItem(inf, "001", "PRODUTO DE TESTE A", 2m, 50m);
        AdicionarItem(inf, "002", "PRODUTO DE TESTE B", 1m, 25m);
        PreencherTotais(inf);
        inf.transp.modFrete = 9;
        var pagamento = inf.pag.detPag.Add();
        pagamento.tPag = 1;
        pagamento.vPag = inf.total.ICMSTot.vNF;
        inf.infAdic.infCpl = "EXEMPLO DIDATICO SEM VALOR FISCAL. NAO TRANSMITIDO.";
    }

    static decimal Moeda(decimal valor) => Math.Round(valor, 2, MidpointRounding.AwayFromZero);
    static void AdicionarItem(InfNFe inf, string codigo, string descricao, decimal quantidade, decimal unitario)
    {
        decimal valor = Moeda(quantidade * unitario);
        decimal icms = Moeda(valor * 0.18m);
        decimal baseRtc = valor - icms;
        decimal ibs = Moeda(baseRtc * 0.001m);
        decimal cbs = Moeda(baseRtc * 0.009m);
        var det = inf.det.Add(); // Já pertence à nota; não existe a etapa obj/add do C++.
        var prod = det.prod;
        prod.nItem = inf.det.Count;
        prod.cProd = codigo;
        prod.cEAN = "SEM GTIN";
        prod.xProd = descricao;
        prod.NCM = "61091000";
        prod.CFOP = "5102";
        prod.uCom = "UN";
        prod.qCom = (double)quantidade;
        prod.vUnCom = (double)unitario;
        prod.vProd = (double)valor;
        prod.cEANTrib = "SEM GTIN";
        prod.uTrib = "UN";
        prod.qTrib = (double)quantidade;
        prod.vUnTrib = (double)unitario;
        prod.indTot = 1;
        var imposto = det.imposto;
        imposto.ICMS.orig = 0;
        imposto.ICMS.CST = 0; // Enum interno CstICMS.ICMS00 = 0.
        imposto.ICMS.modBC = 3;
        imposto.ICMS.vBC = (double)valor;
        imposto.ICMS.pICMS = 18;
        imposto.ICMS.vICMS = (double)icms;
        imposto.PIS.CST = 49;
        imposto.COFINS.CST = 49;
        // Cenário RTC didático reproduzido do exemplo C++ do núcleo, não parametrização fiscal.
        imposto.IBSCBS.CST = "000";
        imposto.IBSCBS.cClassTrib = "000001";
        var rtc = imposto.IBSCBS.gIBSCBS;
        rtc.vBC = (double)baseRtc;
        rtc.gIBSUF.pIBSUF = 0.1;
        rtc.gIBSUF.vIBSUF = (double)ibs;
        rtc.gIBSMun.pIBSMun = 0;
        rtc.gIBSMun.vIBSMun = 0;
        rtc.vIBS = (double)ibs;
        rtc.gCBS.pCBS = 0.9;
        rtc.gCBS.vCBS = (double)cbs;
    }

    static void PreencherTotais(InfNFe inf)
    {
        decimal produtos=0, icms=0, baseRtc=0, ibs=0, cbs=0;
        for (int i=0; i<inf.det.Count; i++)
        {
            var item = inf.det[i];
            produtos += (decimal)item.prod.vProd;
            icms += (decimal)item.imposto.ICMS.vICMS;
            var rtc = item.imposto.IBSCBS.gIBSCBS;
            baseRtc += (decimal)rtc.vBC;
            ibs += (decimal)rtc.vIBS;
            cbs += (decimal)rtc.gCBS.vCBS;
        }
        var total = inf.total.ICMSTot;
        total.vBC = (double)produtos;
        total.vICMS = (double)icms;
        total.vProd = (double)produtos;
        total.vNF = (double)produtos;
        var tributos = inf.total.IBSCBSTot;
        tributos.vBCIBSCBS = (double)baseRtc;
        tributos.gIBS.gIBSUF.vIBSUF = (double)ibs;
        tributos.gIBS.gIBSMun.vIBSMun = 0;
        tributos.gIBS.vIBS = (double)ibs;
        tributos.gCBS.vCBS = (double)cbs;
    }

    static void ConferirLeitura(string xml)
    {
        using var leitura = new Session();
        leitura.Root.notafiscal.Execute("importar",xml);
        var nota = leitura.Root.notafiscal.NFe[0];
        if(nota.infNFe.det.Count != 2 || nota.infNFe.det[1].prod.cProd != "002" || nota.infNFe.total.ICMSTot.vNF != 125)
            throw new InvalidOperationException("Falha na conferência do XML relido");
    }
}
