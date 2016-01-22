#include "odczyt_xml.h"

#include <iostream>


Odczyt_xml::Odczyt_xml()
{
    otworz_plik_xml();
}

Odczyt_xml::~Odczyt_xml()
{

}

bool Odczyt_xml::otworz_plik_xml(){
    static const char *baza = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<AppSettings>"
            " </AppSettings>"
            "<Objekty>"
                "<Ob1>"
                    "<A>-0.6</A>"
                    "<B>0.4</B>"
                    "<k>2</k>"
                    "<var>0.1</var>"
                " </Ob1>"
            " </Objekty>"
            "<Regulatory>"
            " </Regulatory>";
    xmlDoc.LoadFile("appConfig.xml");
    if(xmlDoc.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND){
        xmlDoc.Parse(baza);
        xmlDoc.SaveFile("appConfig.xml");
        xmlDoc.LoadFile("appConfig.xml");
        return true;
    }
    if(xmlDoc.ErrorID()){
        std::string exceptionInfo = "Błąd odczytu pliku konfiguracyjnego.";
        throw exceptionInfo;
        return false;
    }
    return true;
}

std::map< std::string, std::string > Odczyt_xml::odczytaj_konfiguracje_obiektu(){

    std::map< std::string, std::string > parametryObiektu;

    if(otworz_plik_xml()){
        tinyxml2::XMLElement *settings = xmlDoc.FirstChildElement("Objekty")->FirstChildElement("Ob1");
        parametryObiektu["A"] = settings->FirstChildElement("A")->GetText();
        parametryObiektu["B"] = settings->FirstChildElement("B")->GetText();
        parametryObiektu["k"] = settings->FirstChildElement("k")->GetText();
        parametryObiektu["var"] = settings->FirstChildElement("var")->GetText();
        parametryObiektu["zakl"] = settings->FirstChildElement("zakl")->GetText();
    }

    return parametryObiektu;
}

std::string Odczyt_xml::odczytaj_typ_regulatora(){
    std::string typ = "Z";
    if(otworz_plik_xml()){
        tinyxml2::XMLElement *settings = xmlDoc.FirstChildElement("Regulatory")->FirstChildElement();
        typ = settings->FirstAttribute()->Value();
    }
    return typ;
}
