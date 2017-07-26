#include "sockettest.h"
#include <QtCore/QRegularExpression>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>

SocketTest::SocketTest(QObject *parent) : QObject(parent), socket(0), state(0), id(0)
{
}

void SocketTest::Test()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(connected()));

    qDebug() << "Connecting ...";

    socket->connectToHost("dxusa.net", 7300);

    //if(!socket->waitForDisconnected(1000))
    //{
    //    qDebug() << "Error : " << socket->errorString();
    //}
}

bool SocketTest::createDatabaseConnection()
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("spots.sqlite");
//    if (!db.open())
//    {
//        std::cout << "Cannot open database.\n";
//        return false;
//    }

//    QSqlQuery query;
//    query.exec("create table if not exists spots "
//               "(id int primary key, spotter varchar(12), spottercountry varchar(20), frequency varchar(12), call varchar(12), message varchar(30), time varchar(4), grid varchar(4), country varchar(20), band varchar(4), mode varchar(5))");


//    QSqlQuery query1("select call from spots");
//    while (query1.next())
//    {
//        QString call = query1.value(0).toString();
//        qDebug() << call;
//    }

    return true;
}

void SocketTest::connected()
{
    qDebug() << "Connected!";
}

void SocketTest::disconnected()
{
    qDebug() << "Disconnected!";
}

void SocketTest::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

void SocketTest::readyRead()
{
    QByteArray& data =  socket->readAll();
    if (state == 0)
    {
        socket->write("OG2Z\n");
        state = 1;
        return;
    }

    QString line = data.data();
    QRegularExpression expression("DX de (\\S+):\\s*(\\d+.\\d)\\s*(\\S+)\\s*(.*)(\\d\\d\\d\\d)Z");
    QRegularExpressionMatch match = expression.match(line);
    if (!match.hasMatch())
        return;

    QString spotter = match.captured(1);
    QString freq = match.captured(2);
    QString call = match.captured(3);
    QString message = match.captured(4).trimmed();
    QString time = match.captured(5);
    QString grid = "";
    QString country = Country(call);
    BandMode bandMode = Band(freq);
    QString spotterCountry = Country(spotter);

    // qDebug() << country << bandMode.band << call << bandMode.mode;

    QSqlQuery query;
    query.exec(QString("select id from entity where entity='%1'").arg(country));

    if (!query.next())
        return;

    int id = query.value(0).toInt();

    query.exec(QString("select mixed from dxcc where entity=%1").arg(id));
    if (!query.next())
        return;

    if (query.value(0).toInt() == 0)
    {
        qDebug() << "NEW DXCC: "<< country << bandMode.band << call << bandMode.mode;
        return;
    }

    if (bandMode.mode == "phone")
    {
        query.exec(QString("select phone from dxcc where entity=%1").arg(id));
        if (!query.next())
            return;
        if (query.value(0).toInt() == 0)
        {
            qDebug() << "NEW MODE: "<< country << bandMode.band << call << bandMode.mode;
            return;
        }
    }
    else if (bandMode.mode == "cw")
    {
        query.exec(QString("select cw from dxcc where entity=%1").arg(id));
        if (!query.next())
            return;
        if (query.value(0).toInt() == 0)
        {
            qDebug() << "NEW MODE: "<< country << bandMode.band << call << bandMode.mode;
            return;
        }
    }
    else
    {
        query.exec(QString("select data from dxcc where entity=%1").arg(id));
        if (!query.next())
            return;
        if (query.value(0).toInt() == 0)
        {
            qDebug() << "NEW MODE: "<< country << bandMode.band << call << bandMode.mode;
            return;
        }
    }

    query.exec(QString("select '%1' from dxcc where entity=%2").arg(bandMode.band).arg(id));
    if (!query.next())
        return;
    if (query.value(0).toInt() == 0)
    {
        qDebug() << "NEW BAND: "<< country << bandMode.band << call << bandMode.mode;
        return;
    }
}

void SocketTest::findMaximumId()
{
    QSqlQuery query("select id from spots");
    while (query.next())
    {
        int id = query.value(0).toInt();
        if (id > this->id)
            this->id = id;
    }
}

struct DXCC
{
  const char *pattern;
  const char *country;
}

dxcc[] =
{
  {"^1S",                  "SPRATLY ISLANDS"},
  {"^1A",                  "SOV MILITARY ORDER OF MALTA"},
  {"^3A",                  "MONACO"},
  {"^3B[6-7]",             "AGALEGA & ST BRANDON ISLANDS"},
  {"^3B8",                 "MAURITIUS ISLAND"},
  {"^3B9",                 "RODRIGUEZ ISLAND"},
  {"^3C0",                 "ANNOBON"},
  {"^3C",                  "EQUATORIAL GUINEA"},
  {"^3D2.*/C",             "CONWAY REEF"},
  {"^3D2.*/R",             "ROTUMA"},
  {"^3D2",                 "FIJI ISLANDS"},
  {"^3DA",                 "SWAZILAND"},
  {"^3V",                  "TUNISIA"},
  {"^3W|^XV",              "VIET NAM"},
  {"^3X",                  "GUINEA"},
  {"^3Y.*/B",              "BOUVET ISLAND"},
  {"^3Y.*/P",              "PETER 1 ISLAND"},
  {"^4J|^4K",              "AZERBAIJAN"},
  {"^4L",                  "GEORGIA"},
  {"^4O",                  "MONTENEGRO"},
  {"^4S",                  "SRI LANKA"},
  {"^4U1ITU",              "ITU HQ"},
  {"^4U1UN",               "UNITED NATIONS HQ"},
  {"^4W",                  "TIMOR - LESTE"},
  {"^4X|^4Z",              "ISRAEL"},
  {"^5A",                  "LIBYA"},
  {"^5B|^C4|^P3",          "CYPRUS"},
  {"^5H|^5I",              "TANZANIA"},
  {"^5N",                  "NIGERIA"},
  {"^5R",                  "MADAGASCAR"},
  {"^5T",                  "MAURITANIA"},
  {"^5U",                  "NIGER"},
  {"^5V",                  "TOGO"},
  {"^5W",                  "SAMOA"},
  {"^5X",                  "UGANDA"},
  {"^5Y|^5Z",              "KENYA"},
  {"^6V|^6W",              "SENEGAL"},
  {"^6Y",                  "JAMAICA"},
  {"^7O",                  "YEMEN"},
  {"^7P",                  "LESOTHO"},
  {"^7Q",                  "MALAWI"},
  {"^7[T-Y]",              "ALGERIA"},
  {"^8P",                  "BARBADOS"},
  {"^8Q",                  "MALDIVES"},
  {"^8R"  ,                "GUYANA"},
  {"^9A",                  "CROATIA"},
  {"^9G",                  "GHANA"},
  {"^9H",                  "MALTA"},
  {"^9I|^9J",               "ZAMBIA"},
  {"^9K",                  "KUWAIT"},
  {"^9L",                  "SIERRA LEONE"},
  {"^9M2|^9M4",             "WEST MALAYSIA"},
  {"^9M6|^9M8",             "EAST MALAYSIA"},
  {"^9N",                  "NEPAL"},
  {"^9[Q-T]",              "DEM. REP. OF THE CONGO"},
  {"^9U",                  "BURUNDI"},
  {"^9V",                  "SINGAPORE"},
  {"^9X",                  "RWANDA"},
  {"^9Y|^9Z",               "TRINIDAD & TOBAGO"},
  {"^A2",                  "BOTSWANA"},
  {"^A3",                  "TONGA"},
  {"^A4",                  "OMAN"},
  {"^A5",                  "BHUTAN"},
  {"^A6",                  "UNITED ARAB EMIRATES"},
  {"^A7",                  "QATAR"},
  {"^A9",                  "BAHRAIN"},
  {"^AP",                  "PAKISTAN"},
  {"^BS7",                 "SCARBOROUGH REEF"},
  {"^BV9P",                "PRATAS ISLAND"},
  {"^B[U-X]",              "TAIWAN"},
  {"^B",                   "CHINA"},
  {"^C2",                  "NAURU"},
  {"^C3",                  "ANDORRA"},
  {"^C5",                  "THE GAMBIA"},
  {"^C6",                  "BAHAMAS"},
  {"^C8|^C9",               "MOZAMBIQUE"},
  {"^CE0Y",                "EASTER ISLAND"},
  {"^CE0Z",                "JUAN FERNANDEZ ISLAND"},
  {"^CE0X",                "SAN FELIX ISLAND"},
  {"^CE9|^KC4",            "ANTARCTICA"},
  {"^C[A-E]",              "CHILE"},
  {"^CM|^CO",              "CUBA"},
  {"^CN",                  "MOROCCO"},
  {"^CP",                  "BOLIVIA"},
  {"^CT3",                 "MADEIRA ISLANDS"},
  {"^CT",                  "PORTUGAL"},
  {"^CU",                  "AZORES"},
  {"^C[V-X]",              "URUGUAY"},
  {"^CY0",                 "SABLE ISLAND"},
  {"^CY9",                 "SAINT PAUL ISLAND"},
  {"^D2|^D3",              "ANGOLA"},
  {"^D4",                  "CAPE VERDE"},
  {"^D6",                  "COMOROS"},
  {"^D[A-R]",              "FEDERAL REPUBLIC OF GERMANY"},
  {"^D[U-Z]|^4[D-I]",      "PHILIPPINES"},
  {"^E3",                  "ERITREA"},
  {"^E4",                  "PALESTINE"},
  {"^E5.*/N",              "NORTH COOK ISLANDS"},
  {"^E5.*/S",              "SOUTH COOK ISLANDS"},
  {"^E5",                  "SOUTH COOK ISLANDS"},
  {"^E6",                  "NIUE"},
  {"^E7",                  "BOSNIA-HERZEGOVINA"},
  {"^E[A-H]6",             "BALEARIC ISLAND"},
  {"^E[A-H]8",             "CANARY ISLANDS"},
  {"^E[A-H]9",             "CEUTA & MELILLA"},
  {"^E[A-H]",              "SPAIN"},
  {"^EI|EJ",               "IRELAND"},
  {"^EK",                  "ARMENIA"},
  {"^EL",                  "LIBERIA"},
  {"^EP|^EQ",              "IRAN"},
  {"^ER",                  "MOLDOVA"},
  {"^ES",                  "ESTONIA"},
  {"^ET",                  "ETHIOPIA"},
  {"^E[U-W]",              "BELARUS"},
  {"^EX",                  "KYRGYZSTAN"},
  {"^EY",                  "TAJIKISTAN"},
  {"^EZ",                  "TURKMENISTAN"},
  {"^FG",                  "GUADELOUPE"},
  {"^FH",                  "MAYOTTE ISLAND"},
  {"^FJ",                  "SAINT BARTHELEMY"},
  {"^FK.*/C",              "CHESTERFIELD IS."},
  {"^FK",                  "NEW CALEDONIA"},
  {"^FM",                  "MARTINIQUE"},
  {"^FO.*/A",              "AUSTRAL ISLAND"},
  {"^FO.*/C",              "CLIPPERTON ISLAND"},
  {"^FO.*/M",              "MARQUESAS ISLAND"},
  {"^FO",                  "FRENCH POLYNESIA"},
  {"^FP",                  "SAINT PIERRE & MIQUELON"},
  {"^FR.*/G",              "GLORIOSO ISLAND"},
  {"^FR.*/J",              "JUAN DE NOVA, EUROPA"},
  {"^FR.*/T",              "TROMELIN ISLAND"},
  {"^FR",                  "REUNION ISLAND"},
  {"^FS",                  "SAINT MARTIN"},
  {"^FT5W",                "CROZET ISLAND"},
  {"^FT5X",                "KERGUELEN ISLAND"},
  {"^FT5Z",                "AMSTERDAM & ST PAUL ISLAND"},
  {"^FW",                  "WALLIS & FUTUNA ISLAND"},
  {"^FY",                  "FRENCH GUIANA"},
  {"^F|^TM",               "FRANCE"},
  {"^GD",                  "ISLE OF MAN"},
  {"^[G|M]I",              "NORTHERN IRELAND"},
  {"^[G|M]J",              "JERSEY"},
  {"^[G|M]M",              "SCOTLAND"},
  {"^GU",                  "GUERNSEY"},
  {"^[G|M]W",              "WALES"},
  {"^G|^GX|^M",            "ENGLAND"},
  {"^H40",                 "TEMOTU PROVINCE"},
  {"^H4",                  "SOLOMON ISLANDS"},
  {"^HA|^HG",              "HUNGARY"},
  {"^HB0",                 "LIECHTENSTEIN"},
  {"^HB",                  "SWITZERLAND"},
  {"^HC8|^HD8",            "GALAPAGOS ISLANDS"},
  {"^HC|^HD",              "ECUADOR"},
  {"^HH",                  "HAITI"},
  {"^HI",                  "DOMINICAN REPUBLIC"},
  {"^HK0.*/M",             "MALPELO ISLAND"},
  {"^HK0S",                "SAN ANDRES ISLAND"},
  {"^HJ|^HK|^5J|^5K",      "COLOMBIA"},
  {"^HL|^DS",              "REPUBLIC OF KOREA"},
  {"^HO|^HP",              "PANAMA"},
  {"^HQ|^HR",              "HONDURAS"},
  {"^HS|^E2",              "THAILAND"},
  {"^HV",                  "VATICAN CITY"},
  {"^HZ",                  "SAUDI ARABIA"},
  {"^IS0|^IM0",            "SARDINIA"},
  {"^I",                   "ITALY"},
  {"^J2",                  "DJIBOUTI"},
  {"^J3",                  "GRENADA"},
  {"^J5",                  "GUINEA-BISSAU"},
  {"^J6",                  "SAINT LUCIA"},
  {"^J7",                  "DOMINICA"},
  {"^J8",                  "SAINT VINCENT"},
  {"^JD1.*/M",             "MINAMI TORISHIMA"},
  {"^JD1.*/O",             "OGASAWARA"},
  {"^J[A-S]|^7[J-N]",      "JAPAN"},
  {"^J[T-V]",              "MONGOLIA"},
  {"^JW",                  "SVALBARD"},
  {"^JX",                  "JAN MAYEN"},
  {"^JY",                  "JORDAN"},
  {"^KG4",                 "GUANTANAMO BAY"},
  {"^KH0",                 "MARIANA ISLANDS"},
  {"^KH1",                 "BAKER HOWLAND ISLAND"},
  {"^KH2",                 "GUAM"},
  {"^KH3",                 "JOHNSTON ISLAND"},
  {"^KH4",                 "MIDWAY ISLAND"},
  {"^KH5K",                "KINGMAN REEF"},
  {"^KH5",                 "PALMYRA & JARVIS ISLANDS"},
  {"^KH7K",                "KURE ISLAND"},
  {"^KH[6,7]",             "HAWAII"},
  {"^KH8.*/S",              "SWAINS ISLAND"},
  {"^KH8",                 "AMERICAN SAMOA"},
  {"^KH9",                 "WAKE ISLAND"},
  {"^KL|^AL|^NL|^WL",      "ALASKA"},
  {"^KP1",                 "NAVASSA ISLAND"},
  {"^KP2",                 "US VIRGIN ISLANDS"},
  {"^KP[3,4]",             "PUERTO RICO"},
  {"^KP5",                 "DESECHEO ISLAND"},
  {"^K|^W|^N|^A[A-K]",     "UNITED STATES OF AMERICA"},
  {"^L[A-N]",              "NORWAY"},
  {"^L[O-W]" ,             "ARGENTINA"},
  {"^LX",                  "LUXEMBOURG"},
  {"^LY",                  "LITHUANIA"},
  {"^LZ",                  "BULGARIA"},
  {"^O[A-C]",              "PERU"},
  {"^OD",                  "LEBANON"},
  {"^OE",                  "AUSTRIA"},
  {"^OH0",                 "ALAND ISLAND"},
  {"^OJ0",                 "MARKET REEF"},
  {"^O[F-I]",              "FINLAND"},
  {"^O[K-L]",              "CZECH REPUBLIC"},
  {"^OM",                  "SLOVAK REPUBLIC"},
  {"^O[N-T]",              "BELGIUM"},
  {"^O[U-W]|^OZ",          "DENMARK"},
  {"^OX",                  "GREENLAND"},
  {"^OY",                  "FAROE ISLANDS"},
  {"^P2",                  "PAPUA NEW GUINEA"},
  {"^P4",                  "ARUBA"},
  {"^P5",                  "DPRK (NORTH KOREA)"},
  {"^P[A-I]",              "NETHERLANDS"},
  {"^PJ2",                 "CURACAO"},
  {"^PJ4",                 "BONAIRE"},
  {"^PJ[5,6]",             "SABA & ST EUSTATIUS"},
  {"^PJ7",                 "SINT MAARTEN"},
  {"^P[P-Y]0F",            "FERNANDO DE NORONHA"},
  {"^P[P-Y]0S",            "SAINT PETER AND PAUL ROCKS"},
  {"^P[P-Y]0T",            "TRINDADE & MARTIM VAZ ISLANDS"},
  {"^P[P-Y]|^Z[V-Z]",      "BRAZIL"},
  {"^PZ",                  "SURINAME"},
  {"^R1F",                 "FRANZ JOSEF LAND"},
  {"^S0",                  "WESTERN SAHARA"},
  {"^S2",                  "BANGLADESH"},
  {"^S5",                  "SLOVENIA"},
  {"^S7",                  "SEYCHELLES ISLANDS"},
  {"^S9",                  "SAO TOME & PRINCIPE"},
  {"^S[A-M]|^7S|^8S",      "SWEDEN"},
  {"^S[N-R]|^HF",          "POLAND"},
  {"^ST",                  "SUDAN"},
  {"^SU",                  "EGYPT"},
  {"^SV.*/A",              "MOUNT ATHOS"},
  {"^SV5|^J45",            "DODECANESE"},
  {"^SV9|^J49",            "CRETE"},
  {"^S[V-Z]|^J4",          "GREECE"},
  {"^T2",                  "TUVALU"},
  {"^T30",                 "WESTERN KIRIBATI"},
  {"^T31",                 "CENTRAL KIRIBATI"},
  {"^T32",                 "EASTERN KIRIBATI"},
  {"^T33",                 "BANABA ISLAND"},
  {"^T5|^6O",              "SOMALIA"},
  {"^T7",                  "SAN MARINO"},
  {"^T8",                  "PALAU"},
  {"^T[A-C]",              "TURKEY"},
  {"^TF",                  "ICELAND"},
  {"^TG|^TD",              "GUATEMALA"},
  {"^TI9",                 "COCOS ISLAND"},
  {"^TI|^TE",              "COSTA RICA"},
  {"^TJ",                  "CAMEROON"},
  {"^TK",                  "CORSICA"},
  {"^TL",                  "CENTRAL AFRICAN REPUBLIC"},
  {"^TN",                  "REPUBLIC OF THE CONGO"},
  {"^TR",                  "GABON"},
  {"^TT",                  "CHAD"},
  {"^TU",                  "COTE D'IVOIRE"},
  {"^TY",                  "BENIN"},
  {"^TZ",                  "MALI"},
  {"^UA2|^RA2",            "KALININGRAD"},
  {"^U[J-M]",              "UZBEKISTAN"},
  {"^U[N-Q]",              "KAZAKHSTAN"},
  {"^U[R-Z]|^E[M-O]",      "UKRAINE"},
  {"^U[A-I][8,9,0]|^R[A-Z][8,9,0]",  "ASIATIC RUSSIA"},
  {"^U[A-I][1-7]|^R[A-Z]|^R[1-7]",   "EUROPEAN RUSSIA"},
  {"^V2",                  "ANTIGUA & BARBUDA"},
  {"^V3",                  "BELIZE"},
  {"^V4",                  "SAINT KITTS & NEVIS"},
  {"^V5",                  "NAMIBIA"},
  {"^V6",                  "MICRONESIA"},
  {"^V7",                  "MARSHALL ISLANDS"},
  {"^V8",                  "BRUNEI"},
  {"^V[A-G]|^VO|^VY",      "CANADA"},
  {"^VK0H",                "HEARD ISLAND"},
  {"^VK0M",                "MACQUARIE ISLAND"},
  {"^VK9C",                "COCOS (KEELING) ISLAND"},
  {"^VK9L",                "LORD HOWE ISLAND"},
  {"^VK9M",                "MELLISH REEF"},
  {"^VK9N",                "NORFOLK ISLAND"},
  {"^VK9W",                "WILLIS ISLAND"},
  {"^VK9X",                "CHRISTMAS ISLAND"},
  {"^VK",                  "AUSTRALIA"},
  {"^VP2E",                "ANGUILLA"},
  {"^VP2M",                "MONTSERRAT"},
  {"^VP2V",                "BRITISH VIRGIN ISLAND"},
  {"^VP5",                 "TURKS & CAICOS ISLANDS"},
  {"^VP6.*/D",             "DUCIE ISLAND"},
  {"^VP6",                 "PITCAIRN ISLAND"},
  {"^VP8G",                "SOUTH GEORGIA ISLAND"},
  {"^VP8O",                "SOUTH ORKNEY ISLANDS"},
  {"^VP8S",                "SOUTH SANDWICH ISLANDS"},
  {"^VP8H",                "SOUTH SHETLAND ISLANDS"},
  {"^VP8",                 "FALKLAND ISLANDS"},
  {"^VP9",                 "BERMUDA"},
  {"^VQ9",                 "CHAGOS ISLAND"},
  {"^VR",                  "HONG KONG"},
  {"^VU4",                 "ANDAMAN & NICOBAR ISLAND"},
  {"^VU7",                 "LAKSHADWEEP ISLANDS"},
  {"^VU",                  "INDIA"},
  {"^XE",                  "MEXICO"},
  {"^XF4",                 "REVILLAGIGEDO"},
  {"^XT",                  "BURKINA FASO"},
  {"^XU",                  "CAMBODIA"},
  {"^XW",                  "LAOS"},
  {"^XX9",                 "MACAO"},
  {"^XY|^XZ",              "MYANMAR"},
  {"^YA|^T6",              "AFGHANISTAN"},
  {"^Y[B-H]",              "INDONESIA"},
  {"^YI",                  "IRAQ"},
  {"^YJ",                  "VANUATU"},
  {"^YK",                  "SYRIA"},
  {"^YL",                  "LATVIA"},
  {"^YN|^H[6-7]|^HT",      "NICARAGUA"},
  {"^Y[O-R]",              "ROMANIA"},
  {"^YS|^HU",              "EL SALVADOR"},
  {"^YT|^YU",              "SERBIA"},
  {"^YV0",                 "AVES ISLAND"},
  {"^Y[V-Y]|^4M",          "VENEZUELA"},
  {"^Z2",                  "ZIMBABWE"},
  {"^Z3",                  "MACEDONIA"},
  {"^Z8",                  "REPUBLIC OF SOUTH SUDAN"},
  {"^ZA",                  "ALBANIA"},
  {"^ZB2",                 "GIBRALTAR"},
  {"^ZC4",                 "U K BASES ON CYPRUS"},
  {"^ZD7",                 "SAINT HELENA"},
  {"^ZD8",                 "ASCENSION ISLAND"},
  {"^ZD9",                 "TRISTAN DA CUNHA & GOUGH IS"},
  {"^ZF",                  "CAYMAN ISLANDS"},
  {"^ZK3",                 "TOKELAU ISLANDS"},
  {"^ZL7",                 "CHATHAM ISLAND"},
  {"^ZL8",                 "KERMADEC ISLAND"},
  {"^ZL9",                 "AUCKLAND & CAMPBELL ISLAND"},
  {"^ZL",                  "NEW ZEALAND"},
  {"^ZP",                  "PARAGUAY"},
  {"^ZS8",                 "PRINCE EDWARD & MARION ISLANDS"},
  {"^ZS",                  "REPUBLIC OF SOUTH AFRICA"}
};

QString SocketTest::Country(const QString& call)
{
  for (int i = 0; i < sizeof(dxcc)/sizeof(DXCC); i++)
  {
    QRegularExpression expression(dxcc[i].pattern);
    QRegularExpressionMatch match = expression.match(call);
    if (match.hasMatch())
      return dxcc[i].country;
  }
  return "no match";
}

BandMode SocketTest::Band(const QString& freq)
{
  double f = freq.toDouble();
  QString mode = "unknown";
  QString band = "unknown";

  if (f >= 3500 && f < 3570)
  {
    mode = "cw";
    band = "m80";
  }
  else if (f >= 3570 && f < 3600)
  {
    mode = "digi";
    band = "m80";
  }
  else if (f >= 3600 && f < 4000)
  {
    mode = "phone";
    band = "m80";
  }
  else if (f >= 7000 && f < 7040)
  {
    mode = "cw";
    band = "m40";
  }
  else if (f >= 7040 && f < 7050)
  {
    mode = "digi";
    band = "m40";
  }
  else if (f >= 7050 && f < 7300)
  {
    mode = "phone";
    band = "m40";
  }
  else if (f >= 10100 && f < 10130)
  {
    mode = "cw";
    band = "30m";
  }
  else if (f >= 10130 && f < 10150)
  {
    mode = "digi";
    band = "m30";
  }  
  else if (f >= 14000 && f < 14070)
  {
    mode = "cw";
    band = "m20";
  }
  else if (f >= 14070 && f < 14095)
  {
    mode = "digi";
    band = "m20";
  }
  else if (f >= 14095 && f < 14350)
  {
    mode = "phone";
    band = "m20";
  }
  else if (f >= 18068 && f < 18100)
  {
    mode = "cw";
    band = "m17";
  }
  else if (f >= 18100 && f < 18105)
  {
    mode = "digi";
    band = "m17";
  }
  else if (f >= 18105 && f < 18168)
  {
    mode = "phone";
    band = "m17";
  }
  else if (f >= 21000 && f < 21070)
  {
    mode = "cw";
    band = "m15";
  }
  else if (f >= 21070 && f < 21100)
  {
    mode = "digi";
    band = "m15";
  }
  else if (f >= 21100 && f < 21450)
  {
    mode = "phone";
    band = "m15";
  }
  else if (f >= 24890 && f < 24920)
  {
    mode = "cw";
    band = "m12";
  }
  else if (f >= 24920 && f < 24930)
  {
    mode = "digi";
    band = "m12";
  }
  else if (f >= 24930 && f < 24990)
  {
    mode = "phone";
    band = "m12";
  }  
  else if (f >= 28000 && f < 28070)
  {
    mode = "cw";
    band = "m10";
  }
  else if (f >= 28070 && f < 28120)
  {
    mode = "digi";
    band = "m10";
  }
  else if (f >= 28120 && f < 28700)
  {
    mode = "phone";
    band = "m10";
  }
  return BandMode(band, mode);
}


