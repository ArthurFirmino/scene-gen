#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>

#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h>

#include "Tags/Shape.hpp"
#include "Tags/Scene.hpp"

namespace fs = std::filesystem;

const char *xml_cstr = R"(
<integrator type="path">
    <integer name="max_depth" value="8"/>
</integrator>

<!-- Instantiate a perspective camera with 80 degrees field of view -->
<sensor type="perspective">
    <float name="fov" value="80"/>

    <!-- Generate an EXR image at HD resolution -->
    <film type="hdrfilm">
        <integer name="width" value="1000"/>
        <integer name="height" value="1000"/>
        <string name="pixel_format" value="rgb"/>
        <rfilter type="box"/>
    </film>
</sensor>

<emitter type="constant">
    <spectrum name="radiance" value="0.25"/>
</emitter>

<!-- Add a ground plane -->
<shape type="disk">
    <transform name="to_world">
        <scale value="1000"/>
        <translate value="0 0 -2"/>
    </transform>
    <bsdf type="diffuse">
        <rgb name="reflectance" value="1 1 1"/>
    </bsdf>
</shape>)";

fs::path rootDir;

int main(int argc, char* argv[])
{
    if (char result[PATH_MAX]; 
        readlink("/proc/self/exe", result, PATH_MAX) != -1)
    {
        rootDir = fs::path(dirname(dirname(result)));
    }
    int seed = 432432423, spp = 64, renderSeed = 0,
    crop_window_size = 0;
    for (int i = 1; (i + 1) < argc; i += 2)
    {
        if (std::string_view(argv[i]) == "--seed")
            seed = std::stoi(argv[i + 1]);
        if (std::string_view(argv[i]) == "--spp")
            spp = std::stoi(argv[i + 1]);
        if (std::string_view(argv[i]) == "--render-seed")
            renderSeed = std::stoi(argv[i+1]);
        if (std::string_view(argv[i]) == "--crop-window-size")
            crop_window_size = std::stoi(argv[i+1]);
    }

    RNG rng(seed);
    auto scene = Procedural::Generate(Procedural::Variant::Scene, rng);

    XMLDocument doc;
    auto e = std::dynamic_pointer_cast<Tag>(scene)->ToXMLElement(doc);

    doc.InsertFirstChild(e);

    // Hardcoded integrator, sensor, film
    {
        XMLDocument src; src.Parse(xml_cstr);
        for (const XMLNode* node = src.FirstChild(); node; 
            node = node->NextSibling())
        {
            doc.FirstChild()->InsertFirstChild(node->DeepClone(&doc));
        }
    }
    for (XMLNode* node = doc.FirstChild()->FirstChild();
        node; node = node->NextSibling())
    {
        if (node->ToElement() 
            && std::string_view(node->ToElement()->Name()) == "sensor")
        {
            XMLElement *pElem = doc.NewElement("sampler");
            pElem->SetAttribute("type", "independent");
            XMLElement *pChildElem = doc.NewElement("integer");
            pChildElem->SetAttribute("name", "sample_count");
            pChildElem->SetAttribute("value", spp);
            pElem->InsertFirstChild(pChildElem);
            pChildElem = doc.NewElement("integer");
            pChildElem->SetAttribute("name", "seed");
            pChildElem->SetAttribute("value", renderSeed);
            pElem->InsertFirstChild(pChildElem);

            if (crop_window_size != 0)
            {
                for (XMLNode* subNode = node->FirstChild();
                    subNode; subNode = subNode->NextSibling())
                {
                    if (subNode->ToElement()
                        && std::string_view(subNode->ToElement()->Name()) == "film")
                    {
                            XMLElement *pCrop = doc.NewElement("integer");
                            pCrop->SetAttribute("name", "crop_width");
                            pCrop->SetAttribute("value", crop_window_size);
                            subNode->ToElement()->InsertFirstChild(pCrop);

                            pCrop = doc.NewElement("integer");
                            pCrop->SetAttribute("name", "crop_height");
                            pCrop->SetAttribute("value", crop_window_size);
                            subNode->ToElement()->InsertFirstChild(pCrop);

                            int x = 250 + rng.NextInt() % (500-crop_window_size);
                            int y = 350 + rng.NextInt() % (300-crop_window_size);

                            pCrop = doc.NewElement("integer");
                            pCrop->SetAttribute("name", "crop_offset_x");
                            pCrop->SetAttribute("value", x);
                            subNode->ToElement()->InsertFirstChild(pCrop);

                            pCrop = doc.NewElement("integer");
                            pCrop->SetAttribute("name", "crop_offset_y");
                            pCrop->SetAttribute("value", y);
                            subNode->ToElement()->InsertFirstChild(pCrop);

                            break;
                    }   
                }
            }

            node->ToElement()->InsertFirstChild(pElem);

            // Insert camera sensor
            {
                pElem = doc.NewElement("transform");
                pElem->SetAttribute("name", "to_world");
                pChildElem = doc.NewElement("lookat");
                pChildElem->SetAttribute("origin", "20 0 7.5");
                pChildElem->SetAttribute("target", "0 0 1");
                pChildElem->SetAttribute("up", "0 0 1");
                pElem->InsertFirstChild(pChildElem);
                node->ToElement()->InsertFirstChild(pElem);
            }
        }
    }

    doc.Print();

    return 0;
}
