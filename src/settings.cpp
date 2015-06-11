#include "settings.h"

Settings * Settings::instance;

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    if(Settings::instance != NULL) delete Settings::instance;
    Settings::instance = this;
    filePath = QStandardPaths::displayName(QStandardPaths::HomeLocation).append("/.tox/settings.ini");
    QFileInfo fi(filePath);
    if(!fi.exists() || !fi.isFile()){
#ifdef DEBUG
        qDebug() << filePath << " does not exists, trying to create it with default values.";
#endif
        QDir d(filePath);
        d.mkpath(d.absolutePath().left(d.absolutePath().lastIndexOf("/")));
        QFile f(filePath);
        f.open(QIODevice::WriteOnly| QIODevice::Truncate| QIODevice::Text);
        f.write("[DHT_Server]\n");
        f.write("dhtServerList\\size=21\n");
        f.write("dhtServerList\\1\\name=Nikolai Toryzin\n");
        f.write("dhtServerList\\1\\userId=951C88B7E75C867418ACDB5D273821372BB5BD652740BCDF623A4FA293E75D2F\n");
        f.write("dhtServerList\\1\\address=192.254.75.98\n");
        f.write("dhtServerList\\1\\port=33445\n");
        f.write("dhtServerList\\2\\name=Nikolai Toryzin\n");
        f.write("dhtServerList\\2\\userId=2A4B50D1D525DA2E669592A20C327B5FAD6C7E5962DC69296F9FEC77C4436E4E\n");
        f.write("dhtServerList\\2\\address=31.7.57.236\n");
        f.write("dhtServerList\\2\\port=443\n");
        f.write("dhtServerList\\3\\name=sonOfRa\n");
        f.write("dhtServerList\\3\\userId=04119E835DF3E78BACF0F84235B300546AF8B936F035185E2A8E9E0A67C8924F\n");
        f.write("dhtServerList\\3\\address=144.76.60.215\n");
        f.write("dhtServerList\\3\\port=33445\n");
        f.write("dhtServerList\\4\\name=stal\n");
        f.write("dhtServerList\\4\\userId=A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074\n");
        f.write("dhtServerList\\4\\address=23.226.230.47\n");
        f.write("dhtServerList\\4\\port=33445\n");
        f.write("dhtServerList\\5\\name=astonex\n");
        f.write("dhtServerList\\5\\userId=10B20C49ACBD968D7C80F2E8438F92EA51F189F4E70CFBBB2C2C8C799E97F03E\n");
        f.write("dhtServerList\\5\\address=178.62.125.224\n");
        f.write("dhtServerList\\5\\port=33445\n");
        f.write("dhtServerList\\6\\name=mousey\n");
        f.write("dhtServerList\\6\\userId=5EB67C51D3FF5A9D528D242B669036ED2A30F8A60E674C45E7D43010CB2E1331\n");
        f.write("dhtServerList\\6\\address=37.187.46.132\n");
        f.write("dhtServerList\\6\\port=33445\n");
        f.write("dhtServerList\\7\\name=SylvieLorxu\n");
        f.write("dhtServerList\\7\\userId=4B2C19E924972CB9B57732FB172F8A8604DE13EEDA2A6234E348983344B23057\n");
        f.write("dhtServerList\\7\\address=178.21.112.187\n");
        f.write("dhtServerList\\7\\port=33445\n");
        f.write("dhtServerList\\8\\name=Munrek\n");
        f.write("dhtServerList\\8\\userId=E398A69646B8CEACA9F0B84F553726C1C49270558C57DF5F3C368F05A7D71354\n");
        f.write("dhtServerList\\8\\address=195.154.119.113\n");
        f.write("dhtServerList\\8\\port=33445\n");
        f.write("dhtServerList\\9\\name=nurupo\n");
        f.write("dhtServerList\\9\\userId=F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67\n");
        f.write("dhtServerList\\9\\address=192.210.149.121\n");
        f.write("dhtServerList\\9\\port=33445\n");
        f.write("dhtServerList\\10\\name=aitjcize\n");
        f.write("dhtServerList\\10\\userId=7F9C31FE850E97CEFD4C4591DF93FC757C7C12549DDD55F8EEAECC34FE76C029\n");
        f.write("dhtServerList\\10\\address=54.199.139.199\n");
        f.write("dhtServerList\\10\\port=33445\n");
        f.write("dhtServerList\\11\\name=Jfreegman\n");
        f.write("dhtServerList\\11\\userId=8CD087E31C67568103E8C2A28653337E90E6B8EDA0D765D57C6B5172B4F1F04C\n");
        f.write("dhtServerList\\11\\address=104.219.184.206\n");
        f.write("dhtServerList\\11\\port=443\n");
        f.write("dhtServerList\\12\\name=bunslow\n");
        f.write("dhtServerList\\12\\userId=93574A3FAB7D612FEA29FD8D67D3DD10DFD07A075A5D62E8AF3DD9F5D0932E11\n");
        f.write("dhtServerList\\12\\address=76.191.23.96\n");
        f.write("dhtServerList\\12\\port=33445\n");
        f.write("dhtServerList\\13\\name=Martin Schröder\n");
        f.write("dhtServerList\\13\\userId=F5A1A38EFB6BD3C2C8AF8B10D85F0F89E931704D349F1D0720C3C4059AF2440A\n");
        f.write("dhtServerList\\13\\address=46.38.239.179\n");
        f.write("dhtServerList\\13\\port=33445\n");
        f.write("dhtServerList\\14\\name=lkwg82\n");
        f.write("dhtServerList\\14\\userId=2C308B4518862740AD9A121598BCA7713AFB25858B747313A4D073E2F6AC506C\n");
        f.write("dhtServerList\\14\\address=144.76.93.230\n");
        f.write("dhtServerList\\14\\port=33445\n");
        f.write("dhtServerList\\15\\name=Impyy\n");
        f.write("dhtServerList\\15\\userId=788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B\n");
        f.write("dhtServerList\\15\\address=178.62.250.138\n");
        f.write("dhtServerList\\15\\port=33445\n");
        f.write("dhtServerList\\16\\name=Thierry Thomas\n");
        f.write("dhtServerList\\16\\userId=7A2306BFBA665E5480AE59B31E116BE9C04DCEFE04D9FE25082316FA34B4DA0Cv\n");
        f.write("dhtServerList\\16\\address=78.225.128.39\n");
        f.write("dhtServerList\\16\\port=33445\n");
        f.write("dhtServerList\\17\\name=Manolis\n");
        f.write("dhtServerList\\17\\userId=461FA3776EF0FA655F1A05477DF1B3B614F7D6B124F7DB1DD4FE3C08B03B640F\n");
        f.write("dhtServerList\\17\\address=130.133.110.14\n");
        f.write("dhtServerList\\17\\port=33445\n");
        f.write("dhtServerList\\18\\name=lawk1\n");
        f.write("dhtServerList\\18\\userId=58D2DE4B169502669941E50780C1630FAA48A0B7026D6F4066C320D47AC6401E\n");
        f.write("dhtServerList\\18\\address=178.62.150.106\n");
        f.write("dhtServerList\\18\\port=33445\n");
        f.write("dhtServerList\\19\\name=noisykeyboard\n");
        f.write("dhtServerList\\19\\userId=5918AC3C06955962A75AD7DF4F80A5D7C34F7DB9E1498D2E0495DE35B3FE8A57\n");
        f.write("dhtServerList\\19\\address=104.167.101.29\n");
        f.write("dhtServerList\\19\\port=33445\n");
        f.write("dhtServerList\\20\\name=aceawan\n");
        f.write("dhtServerList\\20\\userId=391C96CB67AE893D4782B8E4495EB9D89CF1031F48460C06075AA8CE76D50A21\n");
        f.write("dhtServerList\\20\\address=195.154.109.148\n");
        f.write("dhtServerList\\20\\port=33445\n");
        f.write("dhtServerList\\21\\name=pastly\n");
        f.write("dhtServerList\\21\\userId=3E1FFDEB667BFF549F619EC6737834762124F50A89C8D0DBF1DDF64A2DD6CD1B\n");
        f.write("dhtServerList\\21\\address=192.3.173.88\n");
        f.write("dhtServerList\\21\\port=33445\n");
        f.close();
    }
#ifdef DEBUG
    qDebug() << "Loading settings from " << filePath;
#endif
    QSettings s(filePath, QSettings::IniFormat);
    s.beginGroup("DHT_Server");
    int serverListSize = s.beginReadArray("dhtServerList");
    for (int i = 0; i < serverListSize; i ++){
        DhtServer server;
        s.setArrayIndex(i);
        server.name = s.value("name").toString();
        server.userId = s.value("userId").toString();
        server.address = s.value("address").toString();
        server.port = s.value("port").toInt();
        server_list << server;
    }
}

Settings * Settings::getInstance(QObject *parent){
    if (Settings::instance == NULL) Settings::instance = new Settings(parent);
    return Settings::instance;
}

QList<Settings::DhtServer>& Settings::getBootstrapServers(){
    return server_list;
}
