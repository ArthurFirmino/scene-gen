#include "Tag.hpp"

#include <string>
#include <sstream>
#include <iostream>

namespace
{
    const char *Get_Schema_baseName(Schema *S)
    {
        return S->baseName ? S->baseName : 
            (S->parent ? Get_Schema_baseName(S->parent) : nullptr);
    }

    const char *Get_Schema_typeName(Schema *S)
    {
        return S->typeName ? S->typeName : 
           (S->parent ? Get_Schema_typeName(S->parent) : nullptr);
    }

    std::vector<decltype(Schema::attributes) *> Get_Schema_attributes(Schema *S)
    {
        std::vector<decltype(Schema::attributes) *> attributes;
        attributes.push_back(&S->attributes);
        if (S->parent)
        {
            auto parent_attributes = Get_Schema_attributes(S->parent);
            attributes.insert(attributes.end(), 
                parent_attributes.begin(), parent_attributes.end());
        }
        return attributes;
    }
}

XMLElement *Tag_ToXMLElement_Impl(unsigned char *p, Schema &S, XMLDocument &doc)
{   
    XMLElement *pElem = doc.NewElement(Get_Schema_baseName(&S));
    if (strcmp(pElem->Name(), "scene") == 0)
    {
        pElem->SetAttribute("version", "2.0.0");
    }
    if (const char *typeName = Get_Schema_typeName(&S); typeName)
    {
        pElem->SetAttribute("type", typeName);
    }
    static Eigen::IOFormat SimpleFmt(
        Eigen::StreamPrecision, Eigen::DontAlignCols, " ", " ", "", "");
    for (auto attributes : Get_Schema_attributes(&S))
    {
        for (const auto &attribute : *attributes)
        {
            switch(attribute.type)
            {
                case Type::Float:
                {
                    XMLElement *pChildElem = doc.NewElement("float");
                    
                    pChildElem->SetAttribute("name", attribute.name);
                    Float *f = (Float *)attribute.getter((Tag *)p);
                    pChildElem->SetAttribute("value", *f);

                    pElem->InsertEndChild(pChildElem);
                    break;
                }
                case Type::Point:
                {
                    XMLElement *pChildElem = doc.NewElement("point");

                    pChildElem->SetAttribute("name", attribute.name);
                    Point *point = (Point *)attribute.getter((Tag *)p);
                    pChildElem->SetAttribute("x", point->x());
                    pChildElem->SetAttribute("y", point->y());
                    pChildElem->SetAttribute("z", point->z());

                    pElem->InsertEndChild(pChildElem);
                    break;
                }
                case Type::Tag:
                {
                    TagPtr pTag = *(TagPtr *)(attribute.getter((Tag *)p));
                    if (pTag)
                        pElem->InsertEndChild(pTag->ToXMLElement(doc));
                    break;
                }
                case Type::TagVector:
                {
                    TagVector *tagVector = (TagVector *)(attribute.getter((Tag *)p));
                    for (auto &pTag : *tagVector)
                    {
                        if (pTag)
                        {
                            XMLElement *pChildElem = pTag->ToXMLElement(doc);
                            pElem->InsertEndChild(pChildElem);
                        }
                    }
                    break;
                }
                case Type::Spectrum:
                {
                    Spectrum *spectrum = (Spectrum *)attribute.getter((Tag *)p);
                    if (RGB *rgb = std::get_if<RGB>(spectrum); rgb)
                    {
                        XMLElement *pChildElem = doc.NewElement("rgb");

                        pChildElem->SetAttribute("name", attribute.name);
                        std::stringstream ss;
                        ss << rgb->format(SimpleFmt);
                        pChildElem->SetAttribute("value", ss.str().c_str());

                        pElem->InsertEndChild(pChildElem);
                    }
                    else if (TagPtr *ppTag = std::get_if<TagPtr>(spectrum);
                        ppTag && *ppTag)
                    {
                        pElem->InsertEndChild((*ppTag)->ToXMLElement(doc));
                    }
                    break;
                }
                case Type::Variant:
                {
                    Variant *var = (Variant *)attribute.getter((Tag *)p);
                    if (Float *f = std::get_if<Float>(var); f)
                    {
                        XMLElement *pChildElem = doc.NewElement("float");
                    
                        pChildElem->SetAttribute("name", attribute.name);
                        pChildElem->SetAttribute("value", *f);

                        pElem->InsertEndChild(pChildElem);
                    }
                    else if (TagPtr *ppTag = std::get_if<TagPtr>(var);
                        ppTag && *ppTag)
                    {
                        pElem->InsertEndChild((*ppTag)->ToXMLElement(doc));
                    }
                    else if (std::string *str = std::get_if<std::string>(var); str)
                    {
                        XMLElement *pChildElem = doc.NewElement("string");

                        pChildElem->SetAttribute("name", attribute.name);
                        pChildElem->SetAttribute("value", str->c_str());

                        pElem->InsertEndChild(pChildElem);
                    }
                    break;
                }
                case Type::Transform:
                {
                    XMLElement *pChildElem = doc.NewElement("transform");
                    pChildElem->SetAttribute("name", "to_world");
                    {
                        XMLElement *pGrandChildElem = doc.NewElement("matrix");
                        
                        Transform *transform = (Transform *)attribute.getter((Tag *)p);  
                        std::stringstream ss;
                        ss << transform->format(SimpleFmt); 
                        pGrandChildElem->SetAttribute("value", ss.str().c_str());

                        pChildElem->InsertEndChild(pGrandChildElem);
                    }
                    pElem->InsertEndChild(pChildElem);
                    break;
                }
                case Type::String:
                {
                    XMLElement *pChildElem = doc.NewElement("string");

                    pChildElem->SetAttribute("name", attribute.name);
                    std::string *str = (std::string *)attribute.getter((Tag *)p);
                    pChildElem->SetAttribute("value", str->c_str());

                    pElem->InsertEndChild(pChildElem);
                    break;
                }
                default:
                {
                    std::abort();
                }
            }
        }
    } 
    return pElem;
}