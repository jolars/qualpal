#include <map>
#include <string>
#include <vector>

namespace qualpal {

inline std::map<std::string, std::map<std::string, std::vector<std::string>>>
  COLOR_PALETTES = {
    { "ColorBrewer",
      { { "BrBG",
          { "#543005",
            "#8C510A",
            "#BF812D",
            "#DFC27D",
            "#F6E8C3",
            "#F5F5F5",
            "#C7EAE5",
            "#80CDC1",
            "#35978F",
            "#01665E",
            "#003C30" } },
        { "PiYG",
          { "#8E0152",
            "#C51B7D",
            "#DE77AE",
            "#F1B6DA",
            "#FDE0EF",
            "#F7F7F7",
            "#E6F5D0",
            "#B8E186",
            "#7FBC41",
            "#4D9221",
            "#276419" } },
        { "PRGn",
          { "#40004B",
            "#762A83",
            "#9970AB",
            "#C2A5CF",
            "#E7D4E8",
            "#F7F7F7",
            "#D9F0D3",
            "#A6DBA0",
            "#5AAE61",
            "#1B7837",
            "#00441B" } },
        { "PuOr",
          { "#7F3B08",
            "#B35806",
            "#E08214",
            "#FDB863",
            "#FEE0B6",
            "#F7F7F7",
            "#D8DAEB",
            "#B2ABD2",
            "#8073AC",
            "#542788",
            "#2D004B" } },
        { "RdBu",
          { "#67001F",
            "#B2182B",
            "#D6604D",
            "#F4A582",
            "#FDDBC7",
            "#F7F7F7",
            "#D1E5F0",
            "#92C5DE",
            "#4393C3",
            "#2166AC",
            "#053061" } },
        { "RdGy",
          { "#67001F",
            "#B2182B",
            "#D6604D",
            "#F4A582",
            "#FDDBC7",
            "#FFFFFF",
            "#E0E0E0",
            "#BABABA",
            "#878787",
            "#4D4D4D",
            "#1A1A1A" } },
        { "RdYlBu",
          { "#A50026",
            "#D73027",
            "#F46D43",
            "#FDAE61",
            "#FEE090",
            "#FFFFBF",
            "#E0F3F8",
            "#ABD9E9",
            "#74ADD1",
            "#4575B4",
            "#313695" } },
        { "RdYlGn",
          { "#A50026",
            "#D73027",
            "#F46D43",
            "#FDAE61",
            "#FEE08B",
            "#FFFFBF",
            "#D9EF8B",
            "#A6D96A",
            "#66BD63",
            "#1A9850",
            "#006837" } },
        { "Spectral",
          { "#9E0142",
            "#D53E4F",
            "#F46D43",
            "#FDAE61",
            "#FEE08B",
            "#FFFFBF",
            "#E6F598",
            "#ABDDA4",
            "#66C2A5",
            "#3288BD",
            "#5E4FA2" } },
        { "Accent",
          { "#7FC97F",
            "#BEAED4",
            "#FDC086",
            "#FFFF99",
            "#386CB0",
            "#F0027F",
            "#BF5B17",
            "#666666" } },
        { "Dark2",
          { "#1B9E77",
            "#D95F02",
            "#7570B3",
            "#E7298A",
            "#66A61E",
            "#E6AB02",
            "#A6761D",
            "#666666" } },
        { "Paired",
          { "#A6CEE3",
            "#1F78B4",
            "#B2DF8A",
            "#33A02C",
            "#FB9A99",
            "#E31A1C",
            "#FDBF6F",
            "#FF7F00",
            "#CAB2D6",
            "#6A3D9A",
            "#FFFF99",
            "#B15928" } },
        { "Pastel1",
          { "#FBB4AE",
            "#B3CDE3",
            "#CCEBC5",
            "#DECBE4",
            "#FED9A6",
            "#FFFFCC",
            "#E5D8BD",
            "#FDDAEC",
            "#F2F2F2" } },
        { "Pastel2",
          { "#B3E2CD",
            "#FDCDAC",
            "#CBD5E8",
            "#F4CAE4",
            "#E6F5C9",
            "#FFF2AE",
            "#F1E2CC",
            "#CCCCCC" } },
        { "Set1",
          { "#E41A1C",
            "#377EB8",
            "#4DAF4A",
            "#984EA3",
            "#FF7F00",
            "#FFFF33",
            "#A65628",
            "#F781BF",
            "#999999" } },
        { "Set2",
          { "#66C2A5",
            "#FC8D62",
            "#8DA0CB",
            "#E78AC3",
            "#A6D854",
            "#FFD92F",
            "#E5C494",
            "#B3B3B3" } },
        { "Set3",
          { "#8DD3C7",
            "#FFFFB3",
            "#BEBADA",
            "#FB8072",
            "#80B1D3",
            "#FDB462",
            "#B3DE69",
            "#FCCDE5",
            "#D9D9D9",
            "#BC80BD",
            "#CCEBC5",
            "#FFED6F" } },
        { "Blues",
          { "#F7FBFF",
            "#DEEBF7",
            "#C6DBEF",
            "#9ECAE1",
            "#6BAED6",
            "#4292C6",
            "#2171B5",
            "#08519C",
            "#08306B" } },
        { "BuGn",
          { "#F7FCFD",
            "#E5F5F9",
            "#CCECE6",
            "#99D8C9",
            "#66C2A4",
            "#41AE76",
            "#238B45",
            "#006D2C",
            "#00441B" } },
        { "BuPu",
          { "#F7FCFD",
            "#E0ECF4",
            "#BFD3E6",
            "#9EBCDA",
            "#8C96C6",
            "#8C6BB1",
            "#88419D",
            "#810F7C",
            "#4D004B" } },
        { "GnBu",
          { "#F7FCF0",
            "#E0F3DB",
            "#CCEBC5",
            "#A8DDB5",
            "#7BCCC4",
            "#4EB3D3",
            "#2B8CBE",
            "#0868AC",
            "#084081" } },
        { "Greens",
          { "#F7FCF5",
            "#E5F5E0",
            "#C7E9C0",
            "#A1D99B",
            "#74C476",
            "#41AB5D",
            "#238B45",
            "#006D2C",
            "#00441B" } },
        { "Greys",
          { "#FFFFFF",
            "#F0F0F0",
            "#D9D9D9",
            "#BDBDBD",
            "#969696",
            "#737373",
            "#525252",
            "#252525",
            "#000000" } },
        { "Oranges",
          { "#FFF5EB",
            "#FEE6CE",
            "#FDD0A2",
            "#FDAE6B",
            "#FD8D3C",
            "#F16913",
            "#D94801",
            "#A63603",
            "#7F2704" } },
        { "OrRd",
          { "#FFF7EC",
            "#FEE8C8",
            "#FDD49E",
            "#FDBB84",
            "#FC8D59",
            "#EF6548",
            "#D7301F",
            "#B30000",
            "#7F0000" } },
        { "PuBu",
          { "#FFF7FB",
            "#ECE7F2",
            "#D0D1E6",
            "#A6BDDB",
            "#74A9CF",
            "#3690C0",
            "#0570B0",
            "#045A8D",
            "#023858" } },
        { "PuBuGn",
          { "#FFF7FB",
            "#ECE2F0",
            "#D0D1E6",
            "#A6BDDB",
            "#67A9CF",
            "#3690C0",
            "#02818A",
            "#016C59",
            "#014636" } },
        { "PuRd",
          { "#F7F4F9",
            "#E7E1EF",
            "#D4B9DA",
            "#C994C7",
            "#DF65B0",
            "#E7298A",
            "#CE1256",
            "#980043",
            "#67001F" } },
        { "Purples",
          { "#FCFBFD",
            "#EFEDF5",
            "#DADAEB",
            "#BCBDDC",
            "#9E9AC8",
            "#807DBA",
            "#6A51A3",
            "#54278F",
            "#3F007D" } },
        { "RdPu",
          { "#FFF7F3",
            "#FDE0DD",
            "#FCC5C0",
            "#FA9FB5",
            "#F768A1",
            "#DD3497",
            "#AE017E",
            "#7A0177",
            "#49006A" } },
        { "Reds",
          { "#FFF5F0",
            "#FEE0D2",
            "#FCBBA1",
            "#FC9272",
            "#FB6A4A",
            "#EF3B2C",
            "#CB181D",
            "#A50F15",
            "#67000D" } },
        { "YlGn",
          { "#FFFFE5",
            "#F7FCB9",
            "#D9F0A3",
            "#ADDD8E",
            "#78C679",
            "#41AB5D",
            "#238443",
            "#006837",
            "#004529" } },
        { "YlGnBu",
          { "#FFFFD9",
            "#EDF8B1",
            "#C7E9B4",
            "#7FCDBB",
            "#41B6C4",
            "#1D91C0",
            "#225EA8",
            "#253494",
            "#081D58" } },
        { "YlOrBr",
          { "#FFFFE5",
            "#FFF7BC",
            "#FEE391",
            "#FEC44F",
            "#FE9929",
            "#EC7014",
            "#CC4C02",
            "#993404",
            "#662506" } },
        { "YlOrRd",
          { "#FFFFCC",
            "#FFEDA0",
            "#FED976",
            "#FEB24C",
            "#FD8D3C",
            "#FC4E2A",
            "#E31A1C",
            "#BD0026",
            "#800026" } } } },
    { "Tableau",
      { { "10",
          { "#4E79A7",
            "#F28E2B",
            "#E15759",
            "#76B7B2",
            "#59A14F",
            "#EDC948",
            "#B07AA1",
            "#FF9DA7",
            "#9C755F",
            "#BAB0AC" } },

        { "20", { "#4E79A7", "#A0CBE8", "#F28E2B", "#FFBE7D", "#59A14F",
                  "#8CD17D", "#B6992D", "#F1CE63", "#499894", "#86BCB6",
                  "#E15759", "#FF9D9A", "#79706E", "#BAB0AC", "#D37295",
                  "#FABFD2", "#B07AA1", "#D4A6C8", "#9D7660", "#D7B5A6" } },
        { "ColorBlind",
          { "#1170aa",
            "#fc7d0b",
            "#a3acb9",
            "#57606c",
            "#5fa2ce",
            "#c85200",
            "#7b848f",
            "#a3cce9",
            "#ffbc79",
            "#c8d0d9" } } } },
    { "Stata",
      {
        { "S2Color",
          { "#1a476f",
            "#90353b",
            "#55752f",
            "#e37e00",
            "#6e8e84",
            "#c10534",
            "#938dd2",
            "#cac27e",
            "#a0522d",
            "#7b92a8",
            "#2d6d66",
            "#9c8847",
            "#bfa19c",
            "#ffd200",
            "#d9e6eb" } },
        { "S1Rcolor",
          { "#ffff00",
            "#00ff00",
            "#0080ff",
            "#ff00ff",
            "#ff7f00",
            "#ff0000",
            "#add8e6",
            "#ffe474",
            "#00ff80",
            "#c0dcc0",
            "#ff4500",
            "#0000ff",
            "#ff0080",
            "#6e8e84",
            "#a0522d" } },
        { "S1Color",
          { "#006000",
            "#ff4500",
            "#1a476f",
            "#90353b",
            "#6e8e84",
            "#a0522d",
            "#ff7f00",
            "#ff00ff",
            "#00ffff",
            "#ff0000",
            "#00ff00",
            "#9c8847",
            "#800080",
            "#c0dcc0",
            "#add8e6" } },
        { "Economist",
          { "#7b92a8",
            "#82c0e9",
            "#2d6d66",
            "#bfa19c",
            "#008bbc",
            "#97b6b0",
            "#d7d29e",
            "#1a476f",
            "#90353b",
            "#9c8847",
            "#938dd2",
            "#6e8e84",
            "#c10534",
            "#cac27e" } },
      } },
    { "Vermeer",
      { { "Milkmaid",
          { "#48211A",
            "#376597",
            "#537270",
            "#556246",
            "#928F6B",
            "#CCAF69",
            "#ECE5D3",
            "#104A8A",
            "#C5AC8E",
            "#889F95",
            "#7C4728",
            "#00295D",
            "#D9B196" } },
        { "PearlEarring",
          { "#A65141",
            "#E7CDC2",
            "#80A0C7",
            "#394165",
            "#FCF9F0",
            "#B1934A",
            "#DCA258",
            "#100F14",
            "#8B9DAF",
            "#EEDA9D",
            "#E8DCCF" } },
        { "ViewOfDelft",
          { "#78A8D1",
            "#D5BF98",
            "#E3C78F",
            "#FDF9F8",
            "#867D6C",
            "#A04437",
            "#AF7366",
            "#EFBF6A",
            "#8E7C56",
            "#CDD4E4",
            "#8B6C4F",
            "#B4B7B9" } },
        { "LittleStreet",
          { "#9D3D2D",
            "#99A8A5",
            "#8F4C2D",
            "#6A6A54",
            "#EADAC5",
            "#27221F",
            "#727C73",
            "#C9AA82",
            "#3A2D22",
            "#8F5144",
            "#C9BDA3" } } } },
    { "Rembrandt",
      { { "AnatomyLesson",
          { "#B2AAA2",
            "#0D0B0C",
            "#B47562",
            "#332826",
            "#B18147",
            "#803B31",
            "#CBC2C3" } },
        { "Staalmeesters",
          { "#A13826",
            "#701B06",
            "#4C3114",
            "#7A491E",
            "#D7E1D6",
            "#060A0D",
            "#D39C7B" } } } },
    { "Pokemon",
      {

        { "Charizard",
          { "#D86020",
            "#F89040",
            "#903000",
            "#184068",
            "#F8D068",
            "#207890",
            "#C03020",
            "#F8C060",
            "#F8A058",
            "#F8E098",
            "#E85040",
            "#F8F8F8",
            "#D0D0D0",
            "#28A8B8" } },
        { "Pidgeotto",
          { "#785848",
            "#E0B048",
            "#D03018",
            "#202020",
            "#A87858",
            "#F8E858",
            "#583828",
            "#E86040",
            "#F0F0A0",
            "#F8A870",
            "#C89878",
            "#F8F8F8",
            "#A0A0A0" } },
        { "Porygon",
          { "#40A0D8",
            "#F89088",
            "#68C8F8",
            "#2078C0",
            "#F8D0A8",
            "#282828",
            "#003098",
            "#D86050",
            "#F8F8F8",
            "#B02010",
            "#D8D8E0" } },
        { "Cyndaquil",
          { "#F86000",
            "#F8E8A0",
            "#E0C868",
            "#405088",
            "#E80000",
            "#886020",
            "#182850",
            "#284060",
            "#6080B8",
            "#F8D800",
            "#B09850",
            "#F89800",
            "#F8F8F8" } },
        { "Quilava",
          { "#F8E8A0",
            "#F86000",
            "#405088",
            "#E80000",
            "#F8D800",
            "#E0C868",
            "#B09850",
            "#886020",
            "#F89800",
            "#182850",
            "#284060",
            "#6080B8",
            "#B0B0B0",
            "#F8F8F8" } },
        { "Typhlosion",
          { "#F8E890",
            "#D0C068",
            "#283860",
            "#405078",
            "#A08848",
            "#E80800",
            "#604810",
            "#000848",
            "#F8F8C0",
            "#6088B0",
            "#F8D800",
            "#F86000",
            "#F89800",
            "#F8F8F8" } },
        { "Totodile",
          { "#3080C0",
            "#68B0E0",
            "#484880",
            "#90D0F8",
            "#F8C050",
            "#C82810",
            "#F85820",
            "#780000",
            "#A88028",
            "#F8F8F8",
            "#B0B0B0" } },
        { "Croconaw",
          { "#58A8E0",
            "#3080C0",
            "#484880",
            "#E0B050",
            "#780000",
            "#F8E060",
            "#C82818",
            "#80C8F8",
            "#987008",
            "#F85820",
            "#E04020",
            "#F8A858",
            "#F8F8F8" } },
        { "Feraligatr",
          { "#58A8E0",
            "#3080C0",
            "#484880",
            "#80C8F8",
            "#F85820",
            "#C82818",
            "#F8D050",
            "#C8A840",
            "#780000",
            "#987008",
            "#F8F898",
            "#F8A858",
            "#F8F8F8",
            "#C0C0C0" } },
        { "Yanma",
          { "#F87840",
            "#E0F8F8",
            "#D04000",
            "#A0B0D0",
            "#80E820",
            "#708088",
            "#F8D890",
            "#903000",
            "#50B020",
            "#484850",
            "#306800" } },
        { "Murkrow",
          { "#506868",
            "#385050",
            "#284040",
            "#88A0A0",
            "#987820",
            "#F8F850",
            "#D0B028",
            "#684810",
            "#F8F8F8",
            "#E83840",
            "#A81020",
            "#F8A888" } },
        { "Misdreavus",
          { "#709890",
            "#486870",
            "#284840",
            "#A81048",
            "#F890B8",
            "#E85890",
            "#90B8B0",
            "#782000",
            "#F8D0C0",
            "#B82018",
            "#F85828",
            "#F8F050",
            "#F8F8F8" } },
        { "Girafarig",
          { "#585038",
            "#A09050",
            "#F8D840",
            "#E8B030",
            "#786838",
            "#F8F8F8",
            "#403020",
            "#B88810",
            "#E86870",
            "#F8A8B8",
            "#B0D0E0",
            "#F8F078",
            "#D0C888" } },
        { "Dunsparce",
          { "#48A8B8",
            "#E0E080",
            "#B88820",
            "#F0F0C0",
            "#D8B838",
            "#205088",
            "#80C8D0",
            "#685828",
            "#C0C8D0",
            "#585858",
            "#F8F8F8",
            "#888888",
            "#B8D8E0" } },
        { "Surskit",
          { "#305858",
            "#5888B0",
            "#78C0D0",
            "#786810",
            "#207090",
            "#F8D058",
            "#A88810",
            "#58A0D0",
            "#C8A830",
            "#E87888",
            "#E06858",
            "#F8F8F8" } },
        { "Masquerain",
          { "#90A0C0",
            "#C88830",
            "#F8F8F8",
            "#C0E8E8",
            "#F89848",
            "#B0C8E8",
            "#684820",
            "#404058",
            "#D8D8C8",
            "#804020",
            "#181818",
            "#D83820",
            "#581008",
            "#A81008" } },
        { "Ninjask",
          { "#F8F8F8",
            "#000000",
            "#A8B0B0",
            "#889090",
            "#404058",
            "#B03820",
            "#D8A850",
            "#886840",
            "#F0D050",
            "#F06810",
            "#D0D0C8",
            "#585878",
            "#D84810" } },
        { "Loudred",
          { "#7870B0",
            "#585088",
            "#000000",
            "#A098D8",
            "#383868",
            "#F8D858",
            "#D0A018",
            "#C8C8C8",
            "#987018",
            "#F8F8F8",
            "#D04830",
            "#707070",
            "#F87050",
            "#683018" } },
        { "Exploud",
          { "#504888",
            "#000000",
            "#7870B0",
            "#B8B8E8",
            "#A098D8",
            "#D8B048",
            "#F8D858",
            "#D04830",
            "#F87050",
            "#987018",
            "#683018",
            "#983818",
            "#D0D8F8",
            "#F8F8F8",
            "#808080" } },
        { "Medicham",
          { "#C85850",
            "#000000",
            "#E07878",
            "#F89890",
            "#888878",
            "#E8E8D0",
            "#B8B8A8",
            "#785048",
            "#D0D0C0",
            "#F8C858",
            "#F8E080",
            "#A08868",
            "#F8F8F8" } },
        { "Minun",
          { "#5878E0",
            "#F8E8C0",
            "#E8D080",
            "#000000",
            "#6098F8",
            "#305880",
            "#787050",
            "#F8F0D8",
            "#B8A880",
            "#283040",
            "#E85038",
            "#404060",
            "#F8F8F8" } },
        { "Volbeat",
          { "#484850",
            "#000000",
            "#A0B0C0",
            "#604830",
            "#E05060",
            "#D8B850",
            "#7888A0",
            "#F8F8F8",
            "#C83850",
            "#888890",
            "#883048",
            "#F8E050",
            "#D8D8D8",
            "#F08070",
            "#485068" } },
        { "Illumise",
          { "#88A8C8",
            "#484848",
            "#000000",
            "#B060C8",
            "#A8C0E8",
            "#885098",
            "#302830",
            "#787878",
            "#586888",
            "#C888D8",
            "#E0B048",
            "#988050",
            "#F8E070",
            "#F8F8F8",
            "#0058D0" } },
        { "Roselia",
          { "#000000",
            "#285890",
            "#F05880",
            "#78C860",
            "#B0E870",
            "#A03048",
            "#5090D0",
            "#609048",
            "#406030",
            "#8098A0",
            "#60B0F8",
            "#C8D8D0",
            "#A8C0C0",
            "#F8A0B8",
            "#F0D048" } },
        { "Crawdaunt",
          { "#000000",
            "#B84000",
            "#E06800",
            "#F0A830",
            "#883820",
            "#C0A060",
            "#907040",
            "#E0C890",
            "#F8E058",
            "#F0E8C0",
            "#5098F0",
            "#4070A8",
            "#F8F8F8",
            "#184068",
            "#C0C0E0" } },
        { "Armaldo",
          { "#000000",
            "#384050",
            "#8888B8",
            "#D8B060",
            "#585888",
            "#7070A8",
            "#686878",
            "#B8B8D8",
            "#F86048",
            "#D8D8D8",
            "#F8D868",
            "#B0B0B8",
            "#B83800",
            "#F8F8F8" } },
        { "Deoxys",
          { "#181818",
            "#CE6363",
            "#7B7352",
            "#FF844A",
            "#73D6AD",
            "#734A31",
            "#84A59C",
            "#9C9C84",
            "#524A42",
            "#39635A",
            "#FFAD84",
            "#DE6BE7",
            "#8C08CE",
            "#C6C6D6" } },
      } },
    { "Ochre",
      { { "NamatjiraQual",
          { "#d8f0f0",
            "#786060",
            "#d8c0a8",
            "#a86030",
            "#a890a8",
            "#486090",
            "#a89078",
            "#f0a860" } },
        { "NamatjiraDiv",
          { "#786060",
            "#486090",
            "#a890a8",
            "#d8f0f0",
            "#f0a860",
            "#d8c0a8",
            "#a89078",
            "#a86030" } },
        { "Mccrea",
          { "#a8c0a8",
            "#c0c0a8",
            "#a8a890",
            "#909078",
            "#a8a8a8",
            "#c0c0c0",
            "#d8c0a8",
            "#c09048",
            "#c07848",
            "#d89060",
            "#c07848" } },
        { "Parliament",
          { "#a84848",
            "#f0d8d8",
            "#304830",
            "#909078",
            "#607890",
            "#90a8a8",
            "#906030",
            "#c0a890" } },
        { "Tasmania",
          { "#a84830",
            "#d89060",
            "#604830",
            "#6090d8",
            "#c0c0d8",
            "#606030",
            "#486030" } },
        { "NolanNed",
          { "#a89030", "#004878", "#78a890", "#000000", "#481800" } },
        { "Winmar",
          { "#d8d8d8",
            "#609060",
            "#183030",
            "#a8c090",
            "#604848",
            "#d8c0a8",
            "#a86048" } },
        { "OlsenQual",
          { "#c47000",
            "#e0c48c",
            "#8c3800",
            "#e0a81c",
            "#8ca8c4",
            "#385438" } },
        { "OlsenSeq",
          { "#701c00",
            "#8c3800",
            "#a85400",
            "#c47000",
            "#c48c1c",
            "#e0a81c",
            "#e0c48c",
            "#e0e0e0",
            "#c4c4c4",
            "#8ca8c4",
            "#708ca8",
            "#1c54a8",
            "#385438",
            "#38381c" } },
        { "WilliamsPilbara",
          { "#d23c1e",
            "#965a3c",
            "#b4783c",
            "#b4d2d2",
            "#96b4b4",
            "#96b4d2",
            "#7896b4" } },
        { "HealthyReef",
          { "#f63b32",
            "#e37870",
            "#f3ff41",
            "#005487",
            "#52bcd4",
            "#679c40",
            "#f3fbfb",
            "#1c0608",
            "#002B52" } },
        { "EmuWomanPaired",
          { "#AB7E37",
            "#79222A",
            "#562323",
            "#333B43",
            "#B68830",
            "#232728",
            "#B9ACA3",
            "#7A2529",
            "#2C374A",
            "#120F11",
            "#768B99",
            "#6F2827",
            "#7B939C",
            "#BD852C",
            "#AAAEB5",
            "#78292F",
            "#B6B0AE",
            "#3A3A43" } },
        { "Galah",
          { "#903030",
            "#d84860",
            "#f0a8c0",
            "#ffffff",
            "#a8a8a8",
            "#787878" } },
        { "Lorikeet",
          { "#486030",
            "#c03018",
            "#f0a800",
            "#484878",
            "#a8c018",
            "#609048" } },
        { "DeadReef",
          { "#D2E1DE",
            "#548495",
            "#3C4347",
            "#758388",
            "#DBE9E2",
            "#8E9796" } },
        { "JumpingFrog",
          { "#607848", "#184848", "#c0c030", "#a8a890", "#90a878" } } } }
  };

} // namespace qualpal
