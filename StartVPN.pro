QT       += core gui concurrent
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Start
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    userinformation.cpp

HEADERS += \
    mainwindow.h \
    userinformation.h

FORMS += \
    mainwindow.ui

!include(common/common.pri){
   error( "Couldn't find the dependencies.pri!" )
}
!include(core/core.pri){
   error( "Couldn't find the dependencies.pri!" )
}
!include(dependencies.pri) {
    error( "Couldn't find the dependencies.pri!" )
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




DISTFILES += \
    resources/blew-arrow.png \
    resources/flags/flag_afghanistan.png \
    resources/flags/flag_albania.png \
    resources/flags/flag_algeria.png \
    resources/flags/flag_andorra.png \
    resources/flags/flag_angola.png \
    resources/flags/flag_anguilla.png \
    resources/flags/flag_antarctica.png \
    resources/flags/flag_antigua_and_barbuda.png \
    resources/flags/flag_argentina.png \
    resources/flags/flag_armenia.png \
    resources/flags/flag_aruba.png \
    resources/flags/flag_australia.png \
    resources/flags/flag_austria.png \
    resources/flags/flag_azerbaijan.png \
    resources/flags/flag_bahamas.png \
    resources/flags/flag_bahrain.png \
    resources/flags/flag_bangladesh.png \
    resources/flags/flag_barbados.png \
    resources/flags/flag_belarus.png \
    resources/flags/flag_belgium.png \
    resources/flags/flag_belize.png \
    resources/flags/flag_benin.png \
    resources/flags/flag_bermuda.png \
    resources/flags/flag_bhutan.png \
    resources/flags/flag_bolivia.png \
    resources/flags/flag_bosnia.png \
    resources/flags/flag_botswana.png \
    resources/flags/flag_brazil.png \
    resources/flags/flag_british_virgin_islands.png \
    resources/flags/flag_brunei.png \
    resources/flags/flag_bulgaria.png \
    resources/flags/flag_burkina_faso.png \
    resources/flags/flag_burundi.png \
    resources/flags/flag_cambodia.png \
    resources/flags/flag_cameroon.png \
    resources/flags/flag_canada.png \
    resources/flags/flag_cape_verde.png \
    resources/flags/flag_cayman_islands.png \
    resources/flags/flag_central_african_republic.png \
    resources/flags/flag_chad.png \
    resources/flags/flag_chile.png \
    resources/flags/flag_china.png \
    resources/flags/flag_colombia.png \
    resources/flags/flag_comoros.png \
    resources/flags/flag_cook_islands.png \
    resources/flags/flag_costa_rica.png \
    resources/flags/flag_cote_divoire.png \
    resources/flags/flag_croatia.png \
    resources/flags/flag_cuba.png \
    resources/flags/flag_cyprus.png \
    resources/flags/flag_czech_republic.png \
    resources/flags/flag_democratic_republic_of_the_congo.png \
    resources/flags/flag_denmark.png \
    resources/flags/flag_djibouti.png \
    resources/flags/flag_dominica.png \
    resources/flags/flag_dominican_republic.png \
    resources/flags/flag_ecuador.png \
    resources/flags/flag_egypt.png \
    resources/flags/flag_el_salvador.png \
    resources/flags/flag_equatorial_guinea.png \
    resources/flags/flag_eritrea.png \
    resources/flags/flag_estonia.png \
    resources/flags/flag_ethiopia.png \
    resources/flags/flag_falkland_islands.png \
    resources/flags/flag_faroe_islands.png \
    resources/flags/flag_fiji.png \
    resources/flags/flag_finland.png \
    resources/flags/flag_france.png \
    resources/flags/flag_french_polynesia.png \
    resources/flags/flag_gabon.png \
    resources/flags/flag_gambia.png \
    resources/flags/flag_georgia.png \
    resources/flags/flag_germany.png \
    resources/flags/flag_ghana.png \
    resources/flags/flag_gibraltar.png \
    resources/flags/flag_greece.png \
    resources/flags/flag_greenland.png \
    resources/flags/flag_grenada.png \
    resources/flags/flag_guatemala.png \
    resources/flags/flag_guinea.png \
    resources/flags/flag_guinea_bissau.png \
    resources/flags/flag_guyana.png \
    resources/flags/flag_guyane.png \
    resources/flags/flag_haiti.png \
    resources/flags/flag_honduras.png \
    resources/flags/flag_hong_kong.png \
    resources/flags/flag_hungary.png \
    resources/flags/flag_india.png \
    resources/flags/flag_indonesia.png \
    resources/flags/flag_iran.png \
    resources/flags/flag_iraq.png \
    resources/flags/flag_ireland.png \
    resources/flags/flag_isleof_man.png \
    resources/flags/flag_israel.png \
    resources/flags/flag_italy.png \
    resources/flags/flag_jamaica.png \
    resources/flags/flag_japan.png \
    resources/flags/flag_jordan.png \
    resources/flags/flag_kazakhstan.png \
    resources/flags/flag_kenya.png \
    resources/flags/flag_kiribati.png \
    resources/flags/flag_kosovo.png \
    resources/flags/flag_kuwait.png \
    resources/flags/flag_kyrgyzstan.png \
    resources/flags/flag_laos.png \
    resources/flags/flag_latvia.png \
    resources/flags/flag_lebanon.png \
    resources/flags/flag_lesotho.png \
    resources/flags/flag_liberia.png \
    resources/flags/flag_libya.png \
    resources/flags/flag_liechtenstein.png \
    resources/flags/flag_lithuania.png \
    resources/flags/flag_luxembourg.png \
    resources/flags/flag_macao.png \
    resources/flags/flag_macedonia.png \
    resources/flags/flag_madagascar.png \
    resources/flags/flag_malawi.png \
    resources/flags/flag_malaysia.png \
    resources/flags/flag_maldives.png \
    resources/flags/flag_mali.png \
    resources/flags/flag_malta.png \
    resources/flags/flag_marshall_islands.png \
    resources/flags/flag_martinique.png \
    resources/flags/flag_mauritania.png \
    resources/flags/flag_mauritius.png \
    resources/flags/flag_mexico.png \
    resources/flags/flag_micronesia.png \
    resources/flags/flag_moldova.png \
    resources/flags/flag_monaco.png \
    resources/flags/flag_mongolia.png \
    resources/flags/flag_montserrat.png \
    resources/flags/flag_morocco.png \
    resources/flags/flag_mozambique.png \
    resources/flags/flag_myanmar.png \
    resources/flags/flag_namibia.png \
    resources/flags/flag_nauru.png \
    resources/flags/flag_nepal.png \
    resources/flags/flag_netherlands.png \
    resources/flags/flag_new_caledonia.png \
    resources/flags/flag_new_zealand.png \
    resources/flags/flag_nicaragua.png \
    resources/flags/flag_niger.png \
    resources/flags/flag_nigeria.png \
    resources/flags/flag_niue.png \
    resources/flags/flag_north_korea.png \
    resources/flags/flag_norway.png \
    resources/flags/flag_of_montenegro.png \
    resources/flags/flag_oman.png \
    resources/flags/flag_pakistan.png \
    resources/flags/flag_palau.png \
    resources/flags/flag_panama.png \
    resources/flags/flag_papua_new_guinea.png \
    resources/flags/flag_paraguay.png \
    resources/flags/flag_peru.png \
    resources/flags/flag_philippines.png \
    resources/flags/flag_pitcairn_islands.png \
    resources/flags/flag_poland.png \
    resources/flags/flag_portugal.png \
    resources/flags/flag_puerto_rico.png \
    resources/flags/flag_qatar.png \
    resources/flags/flag_republic_of_the_congo.png \
    resources/flags/flag_romania.png \
    resources/flags/flag_russian_federation.png \
    resources/flags/flag_rwanda.png \
    resources/flags/flag_saint_barthelemy.png \
    resources/flags/flag_saint_helena.png \
    resources/flags/flag_saint_kitts_and_nevis.png \
    resources/flags/flag_saint_lucia.png \
    resources/flags/flag_saint_pierre.png \
    resources/flags/flag_saint_vicent_and_the_grenadines.png \
    resources/flags/flag_samoa.png \
    resources/flags/flag_san_marino.png \
    resources/flags/flag_sao_tome_and_principe.png \
    resources/flags/flag_saudi_arabia.png \
    resources/flags/flag_senegal.png \
    resources/flags/flag_serbia.png \
    resources/flags/flag_seychelles.png \
    resources/flags/flag_sierra_leone.png \
    resources/flags/flag_singapore.png \
    resources/flags/flag_slovakia.png \
    resources/flags/flag_slovenia.png \
    resources/flags/flag_soloman_islands.png \
    resources/flags/flag_somalia.png \
    resources/flags/flag_south_africa.png \
    resources/flags/flag_south_korea.png \
    resources/flags/flag_south_sudan.png \
    resources/flags/flag_spain.png \
    resources/flags/flag_sri_lanka.png \
    resources/flags/flag_sudan.png \
    resources/flags/flag_suriname.png \
    resources/flags/flag_swaziland.png \
    resources/flags/flag_sweden.png \
    resources/flags/flag_switzerland.png \
    resources/flags/flag_syria.png \
    resources/flags/flag_taiwan.png \
    resources/flags/flag_tajikistan.png \
    resources/flags/flag_tanzania.png \
    resources/flags/flag_thailand.png \
    resources/flags/flag_timor_leste.png \
    resources/flags/flag_togo.png \
    resources/flags/flag_tokelau.png \
    resources/flags/flag_tonga.png \
    resources/flags/flag_trinidad_and_tobago.png \
    resources/flags/flag_tunisia.png \
    resources/flags/flag_turkey.png \
    resources/flags/flag_turkmenistan.png \
    resources/flags/flag_turks_and_caicos_islands.png \
    resources/flags/flag_tuvalu.png \
    resources/flags/flag_uae.png \
    resources/flags/flag_uganda.png \
    resources/flags/flag_ukraine.png \
    resources/flags/flag_united_kingdom.png \
    resources/flags/flag_united_states_of_america.png \
    resources/flags/flag_uruguay.png \
    resources/flags/flag_us_virgin_islands.png \
    resources/flags/flag_uzbekistan.png \
    resources/flags/flag_vanuatu.png \
    resources/flags/flag_vatican_city.png \
    resources/flags/flag_venezuela.png \
    resources/flags/flag_vietnam.png \
    resources/flags/flag_wallis_and_futuna.png \
    resources/flags/flag_yemen.png \
    resources/flags/flag_zambia.png \
    resources/flags/flag_zimbabuwe.png \
    resources/mac/openvpn \
    resources/mac/openvpn-launcher \
    resources/mac/vpnc-script \
    resources/windows/libcrypto-1_1-x64.dll \
    resources/windows/libcrypto-1_1.dll \
    resources/windows/libeay32.dll \
    resources/windows/liblzo2-2.dll \
    resources/windows/libpkcs11-helper-1.dll \
    resources/windows/libssl-1_1-x64.dll \
    resources/windows/libssl-1_1.dll \
    resources/windows/openvpn.exe \
    resources/windows/ssleay32.dll \
    resources/windows/tap-windows.exe \
    resources/windows/vc_redist.x64.exe \
    resources/windows/vpnc-script.js

RESOURCES += \
    assets.qrc

RC_ICONS = resources\start.ico
