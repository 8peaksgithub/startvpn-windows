#include "common.h"
#include "common/logger.h"
#include "core/openconnectconnection.h"
#include "config.h"
#include <QDateTime>
#include <QMap>
#include <QFile>
#include <QTimeZone>
#include <QRegularExpression>
#include <QApplication>

#ifdef Q_OS_WIN
#include <wbemidl.h>
#include <comdef.h>
#endif

namespace Start {

QMap<QString, int> createCountryCallingCodeMap(){
    QMap<QString, int> map;

    QFile file(":/misc/country_calling_code_map.txt");

    if(file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            auto data = line.split(",");

            auto country = data.at(0);
            country = country.replace("\"", "");
            country = country.simplified();
            country = country.toLower();

            map.insert(country, data.at(1).toInt());
        }
    }

    return map;
}

QString flagIconName(int countryCode)
{
    switch (countryCode) {
    case 4: //afghanistan
        return "flag_afghanistan";
    case 8: //albania
        return "flag_albania";
    case 12: //algeria
        return "flag_algeria";
    case 20: //andorra
        return "flag_andorra";
    case 24: //angola
        return "flag_angola";
    case 10: //antarctica
        return "flag_antarctica";
    case 32: //argentina
        return "flag_argentina";
    case 51: //armenia
        return "flag_armenia";
    case 533: //aruba
        return "flag_aruba";
    case 36: //australia
        return "flag_australia";
    case 40: //austria
        return "flag_austria";
    case 31: //azerbaijan
        return "flag_azerbaijan";
    case 48: //bahrain
        return "flag_bahrain";
    case 50: //bangladesh
        return "flag_bangladesh";
    case 112: //belarus
        return "flag_belarus";
    case 56: //belgium
        return "flag_belgium";
    case 84: //belize
        return "flag_belize";
    case 204: //benin
        return "flag_benin";
    case 64: //bhutan
        return "flag_bhutan";
    case 68: //bolivia, plurinational state of
        return "flag_bolivia";
    case 70: //bosnia and herzegovina
        return "flag_bosnia";
    case 72: //botswana
        return "flag_botswana";
    case 76: //brazil
        return "flag_brazil";
    case 96: //brunei darussalam
        return "flag_brunei";
    case 100: //bulgaria
        return "flag_bulgaria";
    case 854: //burkina faso
        return "flag_burkina_faso";
    case 104: //myanmar
        return "flag_myanmar";
    case 108: //burundi
        return "flag_burundi";
    case 116: //cambodia
        return "flag_cambodia";
    case 120: //cameroon
        return "flag_cameroon";
    case 124: //canada
        return "flag_canada";
    case 132: //cape verde......
        return "flag_cape_verde";
    case 140: //central african republic
        return "flag_central_african_republic";
    case 148: //chad
        return "flag_chad";
    case 152: //chile
        return "flag_chile";
    case 156: //china
        return "flag_china";
    case 170: //colombia
        return "flag_colombia";
    case 174: //comoros
        return "flag_comoros";
    case 178: //congo
        return "flag_republic_of_the_congo";
    case 180: //congo, the democratic republic of the
        return "flag_democratic_republic_of_the_congo";
    case 184: //cook islands
        return "flag_cook_islands";
    case 188: //costa rica
        return "flag_costa_rica";
    case 191: //croatia
        return "flag_croatia";
    case 192: //cuba
        return "flag_cuba";
    case 196: //cyprus
        return "flag_cyprus";
    case 203: //czech republic
        return "flag_czech_republic";
    case 208: //denmark
        return "flag_denmark";
    case 262: //djibouti
        return "flag_djibouti";
    case 626: //timor-leste
        return "flag_timor_leste";
    case 218: //ecuador
        return "flag_ecuador";
    case 818: //egypt
        return "flag_egypt";
    case 222: //el salvador
        return "flag_el_salvador";
    case 226: //equatorial guinea
        return "flag_equatorial_guinea";
    case 232: //eritrea
        return "flag_eritrea";
    case 233: //estonia
        return "flag_estonia";
    case 231: //ethiopia
        return "flag_ethiopia";
    case 238: //falkland islands (malvinas)
        return "flag_falkland_islands";
    case 234: //faroe islands
        return "flag_faroe_islands";
    case 242: //fiji
        return "flag_fiji";
    case 246: //finland
        return "flag_finland";
    case 250: //france
        return "flag_france";
    case 258: //french polynesia
        return "flag_french_polynesia";
    case 266: //gabon
        return "flag_gabon";
    case 270: //gambia
        return "flag_gambia";
    case 268: //georgia
        return "flag_georgia";
    case 276: //germany
        return "flag_germany";
    case 288: //ghana
        return "flag_ghana";
    case 292: //gibraltar
        return "flag_gibraltar";
    case 300: //greece
        return "flag_greece";
    case 304: //greenland
        return "flag_greenland";
    case 320: //guatemala
        return "flag_guatemala";
    case 324: //guinea
        return "flag_guinea";
    case 624: //guinea-bissau
        return "flag_guinea_bissau";
    case 328: //guyana
        return "flag_guyana";
    case 332: //haiti
        return "flag_haiti";
    case 340: //honduras
        return "flag_honduras";
    case 344: //hong kong
        return "flag_hong_kong";
    case 348: //hungary
        return "flag_hungary";
    case 356: //india
        return "flag_india";
    case 360: //indonesia
        return "flag_indonesia";
    case 364: //iran, islamic republic of
        return "flag_iran";
    case 368: //iraq
        return "flag_iraq";
    case 372: //ireland
        return "flag_ireland";
    case 833: //isle of man
        return "flag_isleof_man";
    case 376: //israel
        return "flag_israel";
    case 380: //italy
        return "flag_italy";
    case 384: //côte d\'ivoire
        return "flag_cote_divoire";
    case 392: //japan
        return "flag_japan";
    case 400: //jordan
        return "flag_jordan";
    case 398: //kazakhstan
        return "flag_kazakhstan";
    case 404: //kenya
        return "flag_kenya";
    case 296: //kiribati
        return "flag_kiribati";
    case 414: //kuwait
        return "flag_kuwait";
    case 417: //kyrgyzstan
        return "flag_kyrgyzstan";
    case 136: // Cayman Islands
        return "flag_cayman_islands";
    case 418: //lao people\'s democratic republic
        return "flag_laos";
    case 428: //latvia
        return "flag_latvia";
    case 422: //lebanon
        return "flag_lebanon";
    case 426: //lesotho
        return "flag_lesotho";
    case 430: //liberia
        return "flag_liberia";
    case 434: //libya
        return "flag_libya";
    case 438: //liechtenstein
        return "flag_liechtenstein";
    case 440: //lithuania
        return "flag_lithuania";
    case 442: //luxembourg
        return "flag_luxembourg";
    case 446: //macao
        return "flag_macao";
        //            case 389: //macedonia, the former yugoslav republic of............
        //                return "flag_macedonia";
    case 450: //madagascar
        return "flag_madagascar";
    case 454: //malawi
        return "flag_malawi";
    case 458: //malaysia
        return "flag_malaysia";
    case 462: //maldives
        return "flag_maldives";
    case 466: //mali
        return "flag_mali";
    case 470: //malta
        return "flag_malta";
    case 584: //marshall islands
        return "flag_marshall_islands";
    case 478: //mauritania
        return "flag_mauritania";
    case 480: //mauritius
        return "flag_mauritius";
    case 175: //mayotte
        return "flag_martinique"; // no exact flag found
    case 474: //martinique
        return "flag_martinique";
    case 484: //mexico
        return "flag_mexico";
    case 583: //micronesia, federated states of
        return "flag_micronesia";
    case 498: //moldova, republic of
        return "flag_moldova";
    case 492: //monaco
        return "flag_monaco";
    case 496: //mongolia
        return "flag_mongolia";
    case 499: //montenegro
        return "flag_of_montenegro";// custom
    case 504: //morocco
        return "flag_morocco";
    case 508: //mozambique
        return "flag_mozambique";
    case 516: //namibia
        return "flag_namibia";
    case 520: //nauru
        return "flag_nauru";
    case 524: //nepal
        return "flag_nepal";
    case 528: //netherlands
        return "flag_netherlands";
    case 540: //new caledonia
        return "flag_new_caledonia";
    case 554: //new zealand
        return "flag_new_zealand";
    case 558: //nicaragua
        return "flag_nicaragua";
    case 562: //niger
        return "flag_niger";
    case 566: //nigeria
        return "flag_nigeria";
    case 570: //niue
        return "flag_niue";
    case 410: //north korea
        return "flag_north_korea";
    case 578: //norway
        return "flag_norway";
    case 512: //oman
        return "flag_oman";
    case 586: //pakistan
        return "flag_pakistan";
    case 585: //palau
        return "flag_palau";
    case 591: //panama
        return "flag_panama";
    case 598: //papua new guinea
        return "flag_papua_new_guinea";
    case 600: //paraguay
        return "flag_paraguay";
    case 604: //peru
        return "flag_peru";
    case 608: //philippines
        return "flag_philippines";
    case 612: //pitcairn
        return "flag_pitcairn_islands";
    case 616: //poland
        return "flag_poland";
    case 620: //portugal
        return "flag_portugal";
    case 630: //puerto rico
        return "flag_puerto_rico";
    case 634: //qatar
        return "flag_qatar";
    case 642: //romania
        return "flag_romania";
    case 643: //russian federation
        return "flag_russian_federation";
    case 646: //rwanda
        return "flag_rwanda";
    case 652: //saint barthélemy
        return "flag_saint_barthelemy";// custom
    case 882: //samoa
        return "flag_samoa";
    case 674: //san marino
        return "flag_san_marino";
    case 678: //sao tome and principe
        return "flag_sao_tome_and_principe";
    case 682: //saudi arabia
        return "flag_saudi_arabia";
    case 686: //senegal
        return "flag_senegal";
    case 688: //serbia
        return "flag_serbia"; // custom
    case 690: //seychelles
        return "flag_seychelles";
    case 694: //sierra leone
        return "flag_sierra_leone";
    case 702: //singapore
        return "flag_singapore";
    case 703: //slovakia
        return "flag_slovakia";
    case 705: //slovenia
        return "flag_slovenia";
    case 90: //solomon islands
        return "flag_soloman_islands";
    case 706: //somalia
        return "flag_somalia";
    case 710: //south africa
        return "flag_south_africa";
    case 408: //south korea
        return "flag_south_korea";
    case 724: //spain
        return "flag_spain";
    case 144: //sri lanka
        return "flag_sri_lanka";
    case 654: //saint helena, ascension and tristan da cunha
        return "flag_saint_helena"; // custom
    case 666: //saint pierre and miquelon
        return "flag_saint_pierre";
    case 729: //sudan
        return "flag_sudan";
    case 740: //suriname
        return "flag_suriname";
        //            case 268: //swaziland.......
        //                return "flag_swaziland";
    case 752: //sweden
        return "flag_sweden";
    case 756: //switzerland
        return "flag_switzerland";
    case 760: //syrian arab republic
        return "flag_syria";
    case 158: //taiwan, province of china
        return "flag_taiwan";
    case 762: //tajikistan
        return "flag_tajikistan";
    case 834: //tanzania, united republic of
        return "flag_tanzania";
    case 764: //thailand
        return "flag_thailand";
    case 768: //togo
        return "flag_togo";
    case 772: //tokelau
        return "flag_tokelau";
    case 776: //tonga
        return "flag_tonga";
    case 788: //tunisia
        return "flag_tunisia";
    case 792: //turkey
        return "flag_turkey";
    case 795: //turkmenistan
        return "flag_turkmenistan";
    case 798: //tuvalu
        return "flag_tuvalu";
    case 784: //united arab emirates
        return "flag_uae";
    case 800: //uganda
        return "flag_uganda";
    case 826: //united kingdom
        return "flag_united_kingdom";
    case 804: //ukraine
        return "flag_ukraine";
    case 858: //uruguay
        return "flag_uruguay";
    case 840: //united states
        return "flag_united_states_of_america";
    case 860: //uzbekistan
        return "flag_uzbekistan";
    case 548: //vanuatu
        return "flag_vanuatu";
        //            case va: //holy see (vatican city state)......
        //                return "flag_vatican_city";
    case 862: //venezuela, bolivarian republic of
        return "flag_venezuela";
    case 704: //vietnam
        return "flag_vietnam";
    case 876: //wallis and futuna
        return "flag_wallis_and_futuna";
    case 887: //yemen
        return "flag_yemen";
    case 894: //zambia
        return "flag_zambia";
    case 716: //zimbabwe
        return "flag_zimbabuwe";

    // Caribbean Islands
    case 660: //anguilla
        return "flag_anguilla";
    case 28: //antigua & barbuda
        return "flag_antigua_and_barbuda";
    case 44: //bahamas
        return "flag_bahamas";
    case 52: //barbados
        return "flag_barbados";
    case 92: //british virgin islands
        return "flag_british_virgin_islands";
    case 212: //dominica
        return "flag_dominica";
    case 214: //dominican republic
        return "flag_dominican_republic";
    case 308: //grenada
        return "flag_grenada";
    case 388: //jamaica
        return "flag_jamaica";
    case 500: //montserrat
        return "flag_montserrat";
    case 659: //st kitts & nevis
        return "flag_saint_kitts_and_nevis";
    case 662: //st lucia
        return "flag_saint_lucia";
    case 670: //st vincent & the grenadines
        return "flag_saint_vicent_and_the_grenadines";
    case 780: //trinidad & tobago
        return "flag_trinidad_and_tobago";
    case 796: //turks & caicos islands
        return "flag_turks_and_caicos_islands";
    case 850: //us virgin islands
        return "flag_us_virgin_islands";
    case 728: // south sudan
        return "flag_south_sudan";
        //            case xk: // kosovo.......
        //                return "flag_kosovo";
    case 1000: //us virgin islands
        return "icon_free";
    case 1001: //us virgin islands
        return "icon_high_speed";
    case 1002: //us virgin islands
        return "icon_gaming";
    case 1003: //us virgin islands
        return "icon_streaming";
    default:
        return "";
    }
}

QString Common::appName()
{
    return QObject::tr("Start");
}

QString Common::appVersion()
{
    return "1.0.0";
}

QString Common::appOrganisation()
{
    return "Kolpolok";
}

bool Common::is_url(const QString &str)
{
    if (str.startsWith("system:") || str.startsWith("pkcs11:") || str.startsWith("system:")) {
        return true;
    }
    return false;
}

std::shared_ptr<OpenConnectConnection> Common::globalConnection()
{
    static std::shared_ptr<OpenConnectConnection> conn(new OpenConnectConnection);
    return conn;
}



std::shared_ptr<Config> Common::globalConfig()
{
    static std::shared_ptr<Config> config(new Config);
    return config;
}

QString Common::currentTimeZone()
{
    const auto t1 = QDateTime::currentDateTime();
    const auto t2 = QDateTime(t1.date(), t1.time(),Qt::UTC);

    QString prefix = "+";
    if(t1 > t2)
        prefix = "-";

    auto dt = t1.secsTo(t2);

    QTime t(0,0);
    t = t.addSecs(static_cast<int>(dt));

    auto tz = prefix + t.toString("hh:mm");

    return tz;
}

int Common::currentCountryCallingCode()
{
    static auto map = createCountryCallingCodeMap();

    auto country = QLocale::countryToString(QDateTime::currentDateTime().timeZone().country()).toLower();

    if(map.contains(country))
        return map.value(country);

    return -1;
}

QPair<QString, QString> Common::manufacturerAndModel()
{
    // <Manufacturer, Model>
    QPair<QString,QString> retVal("Unknown", "Unknown");

    // TODO: Implement Cross-platform
#ifdef Q_OS_WIN
    IWbemLocator *locator = nullptr;
    IWbemServices *services = nullptr;
    auto hResult = CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&locator));

    auto hasFailed = [&hResult]() {
        if (FAILED(hResult)) {
            auto error = _com_error(hResult);
            Logger::instance().addMessage(QString::fromWCharArray(error.ErrorMessage()));
            Logger::instance().addMessage(QString::fromWCharArray(error.Description().Detach()));
            return true;
        }
        return false;
    };

    auto getValue = [&hResult, &hasFailed](IWbemClassObject *classObject, LPCWSTR property) {
        QString propertyValueText = "Unknown";
        VARIANT propertyValue;
        hResult = classObject->Get(property, 0, &propertyValue, nullptr, nullptr);
        if (!hasFailed()) {
            if ((propertyValue.vt == VT_NULL) || (propertyValue.vt == VT_EMPTY)) {
            } else if (propertyValue.vt & VT_ARRAY) {
                propertyValueText = "Unknown"; //Array types not supported
            } else {
                propertyValueText = QString::fromWCharArray(propertyValue.bstrVal,static_cast<int>(::SysStringLen(propertyValue.bstrVal)));
            }
        }
        VariantClear(&propertyValue);
        return propertyValueText;
    };

    QString manufacturer = "Unknown";
    QString model = "Unknown";
    if (!hasFailed()) {
        // Connect to the root\cimv2 namespace with the current user and obtain pointer pSvc to make IWbemServices calls.
        hResult = locator->ConnectServer(SysAllocString(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &services);

        if (!hasFailed()) {
            // Set the IWbemServices proxy so that impersonation of the user (client) occurs.
            hResult = CoSetProxyBlanket(services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
                                        RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);

            if (!hasFailed()) {
                IEnumWbemClassObject* classObjectEnumerator = nullptr;
                hResult = services->ExecQuery(SysAllocString(L"WQL"), SysAllocString(L"SELECT * FROM Win32_ComputerSystem"), WBEM_FLAG_FORWARD_ONLY |
                                                                                                                                 WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &classObjectEnumerator);
                if (!hasFailed()) {
                    IWbemClassObject *classObject;
                    ULONG uReturn = 0;
                    hResult = classObjectEnumerator->Next(static_cast<long>(WBEM_INFINITE), 1, &classObject, &uReturn);
                    if (uReturn != 0) {
                        manufacturer = getValue(classObject, static_cast<LPCWSTR>(L"Manufacturer"));
                        model = getValue(classObject, static_cast<LPCWSTR>(L"Model"));
                        retVal.first = manufacturer;
                        retVal.second = model;
                    }
                    classObject->Release();
                }
                classObjectEnumerator->Release();
            }
        }
    }

    if (locator) {
        locator->Release();
    }
    if (services) {
        services->Release();
    }
    CoUninitialize();
#endif

#ifdef Q_OS_MACOS
    retVal.first = "Apple";
    retVal.second = "MacOS";
#endif

    return retVal;
}

int Common::statsInterval()
{
    return 10000;
}

QString Common::flag(int countryCode)
{
    if(countryCode == -1)
        return ":/img/logo.png";

    auto icon = flagIconName(countryCode);
    icon = ":/logo/resources/flags/" + icon;

    return icon;
}

QByteArray Common::updateFontSizes(QByteArray styles, int fontsize1, int fontsize2, int fontWeight)
{
    QString stylesCopy = styles;
    QString compressedStyles = styles.simplified();
    compressedStyles = compressedStyles.replace(" ","");

    // Update font sizes
    {
        QRegularExpression regex("font-size:\\d{1,}px");
        QRegularExpressionMatchIterator i = regex.globalMatch(compressedStyles);

        QSet<QString> fontSizeList;

        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(0);
            fontSizeList.insert(word);
        }

        foreach(auto fontsize, fontSizeList)
        {
            int font = fontsize.replace("font-size:","").replace("px","").simplified().toInt();
            int finalFont = font;

            if(font == 16)
                finalFont = fontsize1;

            if(font == 24)
                finalFont = fontsize2;

            QString variantSource1 = "font-size: " + QString::number(font) + "px";
            QString variantSource2 = "font-size:" + QString::number(font) + "px";

            QString variantTarget = "font-size: " + QString::number(finalFont) + "px";

            stylesCopy = stylesCopy.replace(variantSource1, variantTarget);
            stylesCopy = stylesCopy.replace(variantSource2, variantTarget);
        }
    }

    // Update font weight
    {
        QRegularExpression regex("font-weight:\\d{1,};");
        QRegularExpressionMatchIterator i = regex.globalMatch(compressedStyles);

        QSet<QString> fontWeightList;

        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(0);
            fontWeightList.insert(word);
        }

        foreach(auto w, fontWeightList)
        {
            int weight = w.replace("font-weight:","").replace(";","").simplified().toInt();
            int finalWeight = fontWeight;

            QString variantSource1 = "font-weight: " + QString::number(weight);
            QString variantSource2 = "font-weight:" + QString::number(weight);

            QString variantTarget = "font-weight: " + QString::number(finalWeight);

            stylesCopy = stylesCopy.replace(variantSource1, variantTarget);
            stylesCopy = stylesCopy.replace(variantSource2, variantTarget);
        }
    }

    return stylesCopy.toUtf8();
}


int Common::minorFontSize()
{
    return 14;
}

int Common::majorFontSize()
{
    return 22;
}

int Common::fontWeight()
{
    return 500;
}


} // namespace Symlex
