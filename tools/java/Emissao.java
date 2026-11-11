import crossnfe.Session;
import crossnfe.Models.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.time.OffsetDateTime;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Properties;

/** Mesmo cenário de homologação do exemplo C#. Nenhum comando transmite à SEFAZ. */
public final class Emissao {
    public static void main(String[] args) {
        try {
            if(args.length<1 || args.length>2) throw new IllegalArgumentException("Uso: Emissao <config.properties> [--assinar-validar]");
            boolean assinar=args.length==2 && args[1].equals("--assinar-validar");
            if(args.length==2 && !assinar) throw new IllegalArgumentException("Opção desconhecida");
            var config=new Properties();
            try(var reader=Files.newBufferedReader(Path.of(args[0]),StandardCharsets.UTF_8)){config.load(reader);}
            var saida=Path.of(valor(config,"saida")).toAbsolutePath();
            Files.createDirectories(saida);
            try(var session=new Session()) {
                var root=session.root();
                configurar(root.configuracoes(),assinar);
                var nota=root.notafiscal().NFe().add();
                preencher(nota.infNFe(),config);
                executar(root.notafiscal(),nota,"gerar");
                String chave=nota.get_chNFe();
                Files.writeString(saida.resolve(chave+"-nfe-nao-assinado.xml"),nota.get_XMLOriginal(),StandardCharsets.UTF_8);
                conferirLeitura(nota.get_XMLOriginal());
                if(assinar) {
                    executar(root.notafiscal(),nota,"assinar");
                    Files.writeString(saida.resolve(chave+"-nfe-assinado.xml"),nota.get_XMLAssinado(),StandardCharsets.UTF_8);
                    executar(root.notafiscal(),nota,"validar");
                    System.out.println("XML assinado e validado pelo XSD.");
                } else System.out.println("XML gerado sem assinatura. Use --assinar-validar com certificado e schemas.");
                System.out.println("Chave: "+chave+"\nSaída: "+saida+"\nDocumento NÃO transmitido nem autorizado pela SEFAZ.");
            }
        } catch(Exception e) {System.err.println(e.getMessage());System.exit(1);}
    }
    static String valor(Properties c,String nome) {
        String v=c.getProperty(nome);
        if(v==null || v.isBlank())throw new IllegalArgumentException("Configuração ausente: "+nome);
        return v;
    }
    static void executar(NotaFiscal fiscal,Nfe nota,String operacao) {
        try { fiscal.execute(operacao,""); }
        catch(IllegalStateException e) { throw new IllegalStateException(e.getMessage()+"\n"+nota.get_error(),e); }
    }
    static String ambiente(String nome) {
        String v=System.getenv(nome);
        if(v==null)throw new IllegalArgumentException("Defina "+nome);
        return v;
    }
    // Escapa todos os caracteres de controle JSON; senha nunca é impressa ou gravada.
    static String json(String s) {
        var b=new StringBuilder("\"");
        for(char c:s.toCharArray()) {
            if(c=='"'||c=='\\')b.append('\\').append(c);
            else if(c<32)b.append(String.format("\\u%04x",(int)c));
            else b.append(c);
        }
        return b.append('"').toString();
    }
    static void configurar(ConfigNFe cfg,boolean assinar) {
        cfg.set_ModeloDF(55);
        cfg.set_VersaoNF(0); // Enum interno v400=0, não 400.
        cfg.set_TipoEmissao(1);
        cfg.set_indicadorSincrono(1);
        cfg.webservices().set_tpAmb(2);
        cfg.arquivos().set_salvar(false);
        if(!assinar)return;
        String schemas=ambiente("CROSSNFE_SCHEMAS");
        if(!Files.isRegularFile(Path.of(schemas,"nfe_v4.00.xsd")))throw new IllegalArgumentException("Schema nfe_v4.00.xsd não encontrado");
        cfg.arquivos().set_caminhoSchema(schemas);
        cfg.certificado().execute("carregar","{\"arquivo\":"+json(ambiente("CROSSNFE_PFX"))+",\"senha\":"+json(ambiente("CROSSNFE_PFX_PASSWORD"))+"}");
    }
    static void preencher(InfNFe inf,Properties c) {
        inf.set_versao("4.00");
        var ide=inf.ide();
        ide.set_cUF(35);
        ide.set_cNF(12345678); // Fixo apenas para reproduzir o exemplo.
        ide.set_natOp("VENDA DE MERCADORIA");
        ide.set_mod(55);
        ide.set_serie(Integer.parseInt(valor(c,"serie")));
        ide.set_nNF(Integer.parseInt(valor(c,"numero")));
        ide.set_dhEmi(OffsetDateTime.parse(valor(c,"dataEmissao")).toString());
        ide.set_tpNF(1); ide.set_idDest(1); ide.set_cMunFG(3509502);
        ide.set_tpImp(1); ide.set_tpEmis(1); ide.set_tpAmb(2);
        ide.set_finNFe(1); ide.set_indFinal(1); ide.set_indPres(1);
        ide.set_indIntermed(0); ide.set_procEmi(0); ide.set_verProc("CrossNFe exemplo 0.1");

        var emitente=inf.emite();
        emitente.set_CNPJ(valor(c,"emitente.cnpj"));
        emitente.set_IE(valor(c,"emitente.ie"));
        emitente.set_xNome(valor(c,"emitente.nome"));
        emitente.set_xFant("EMITENTE DE TESTE"); emitente.set_CRT(3);
        var endereco=emitente.enderEmit();
        endereco.set_xLgr("RUA DE TESTE"); endereco.set_nro(100); endereco.set_xBairro("CENTRO");
        endereco.set_cMun(3509502); endereco.set_xMun("CAMPINAS"); endereco.set_UF("SP");
        endereco.set_CEP(13010000); endereco.set_cPais(1058); endereco.set_xPais("BRASIL"); endereco.set_fone("1130000000");

        var dest=inf.dest();
        dest.set_CPF("12345678909");
        dest.set_xNome("NF-E EMITIDA EM AMBIENTE DE HOMOLOGACAO - SEM VALOR FISCAL");
        dest.set_indIEDest(9);
        var entrega=dest.enderDest();
        entrega.set_xLgr("RUA DO DESTINATARIO"); entrega.set_nro(200); entrega.set_xBairro("CENTRO");
        entrega.set_cMun(3509502); entrega.set_xMun("CAMPINAS"); entrega.set_UF("SP");
        entrega.set_CEP(13010000); entrega.set_cPais(1058); entrega.set_xPais("BRASIL");

        adicionarItem(inf,"001","PRODUTO DE TESTE A",new BigDecimal("2"),new BigDecimal("50.00"));
        adicionarItem(inf,"002","PRODUTO DE TESTE B",new BigDecimal("1"),new BigDecimal("25.00"));
        preencherTotais(inf);
        inf.transp().set_modFrete(9);
        var pagamento=inf.pag().detPag().add();
        pagamento.set_tPag(1); pagamento.set_vPag(inf.total().ICMSTot().get_vNF());
        inf.infAdic().set_infCpl("EXEMPLO DIDATICO SEM VALOR FISCAL. NAO TRANSMITIDO.");
    }
    static BigDecimal moeda(BigDecimal v){return v.setScale(2,RoundingMode.HALF_UP);}
    static void adicionarItem(InfNFe inf,String codigo,String descricao,BigDecimal quantidade,BigDecimal unitario) {
        var valor=moeda(quantidade.multiply(unitario));
        var icms=moeda(valor.multiply(new BigDecimal("0.18")));
        var baseRtc=valor.subtract(icms);
        var ibs=moeda(baseRtc.multiply(new BigDecimal("0.001")));
        var cbs=moeda(baseRtc.multiply(new BigDecimal("0.009")));
        var det=inf.det().add(); // Item já pertence à nota.
        var prod=det.prod();
        prod.set_nItem(inf.det().count()); prod.set_cProd(codigo); prod.set_cEAN("SEM GTIN");
        prod.set_xProd(descricao); prod.set_NCM("61091000"); prod.set_CFOP("5102"); prod.set_uCom("UN");
        prod.set_qCom(quantidade.doubleValue()); prod.set_vUnCom(unitario.doubleValue()); prod.set_vProd(valor.doubleValue());
        prod.set_cEANTrib("SEM GTIN"); prod.set_uTrib("UN");
        prod.set_qTrib(quantidade.doubleValue()); prod.set_vUnTrib(unitario.doubleValue()); prod.set_indTot(1);
        var imposto=det.imposto();
        imposto.ICMS().set_orig(0); imposto.ICMS().set_CST(0); // CstICMS.ICMS00 interno = 0.
        imposto.ICMS().set_modBC(3); imposto.ICMS().set_vBC(valor.doubleValue());
        imposto.ICMS().set_pICMS(18); imposto.ICMS().set_vICMS(icms.doubleValue());
        imposto.PIS().set_CST(49); imposto.COFINS().set_CST(49);
        // Massa didática RTC do exemplo do núcleo. Não usar como regra fiscal genérica.
        imposto.IBSCBS().set_CST("000"); imposto.IBSCBS().set_cClassTrib("000001");
        var rtc=imposto.IBSCBS().gIBSCBS();
        rtc.set_vBC(baseRtc.doubleValue());
        rtc.gIBSUF().set_pIBSUF(0.1); rtc.gIBSUF().set_vIBSUF(ibs.doubleValue());
        rtc.gIBSMun().set_pIBSMun(0); rtc.gIBSMun().set_vIBSMun(0);
        rtc.set_vIBS(ibs.doubleValue()); rtc.gCBS().set_pCBS(0.9); rtc.gCBS().set_vCBS(cbs.doubleValue());
    }
    static void preencherTotais(InfNFe inf) {
        BigDecimal produtos=BigDecimal.ZERO,icms=BigDecimal.ZERO,baseRtc=BigDecimal.ZERO,ibs=BigDecimal.ZERO,cbs=BigDecimal.ZERO;
        for(int i=0;i<inf.det().count();i++) {
            var item=inf.det().get(i);
            produtos=produtos.add(BigDecimal.valueOf(item.prod().get_vProd()));
            icms=icms.add(BigDecimal.valueOf(item.imposto().ICMS().get_vICMS()));
            var rtc=item.imposto().IBSCBS().gIBSCBS();
            baseRtc=baseRtc.add(BigDecimal.valueOf(rtc.get_vBC()));
            ibs=ibs.add(BigDecimal.valueOf(rtc.get_vIBS()));
            cbs=cbs.add(BigDecimal.valueOf(rtc.gCBS().get_vCBS()));
        }
        var total=inf.total().ICMSTot();
        total.set_vBC(produtos.doubleValue()); total.set_vICMS(icms.doubleValue());
        total.set_vProd(produtos.doubleValue()); total.set_vNF(produtos.doubleValue());
        var tributos=inf.total().IBSCBSTot();
        tributos.set_vBCIBSCBS(baseRtc.doubleValue()); tributos.gIBS().gIBSUF().set_vIBSUF(ibs.doubleValue());
        tributos.gIBS().gIBSMun().set_vIBSMun(0); tributos.gIBS().set_vIBS(ibs.doubleValue()); tributos.gCBS().set_vCBS(cbs.doubleValue());
    }
    static void conferirLeitura(String xml) {
        try(var leitura=new Session()) {
            leitura.root().notafiscal().execute("importar",xml);
            var nota=leitura.root().notafiscal().NFe().get(0);
            if(nota.infNFe().det().count()!=2 || !nota.infNFe().det().get(1).prod().get_cProd().equals("002") || nota.infNFe().total().ICMSTot().get_vNF()!=125)
                throw new IllegalStateException("Falha na conferência do XML relido");
        }
    }
}
