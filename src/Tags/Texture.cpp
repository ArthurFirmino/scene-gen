#include "Texture.hpp"

#include <filesystem>

namespace fs = std::filesystem;

extern fs::path rootDir;

Schema TextureSchema = 
{
    .baseName = "texture",
    .attributes = 
    {
        { Type::String, "filename", GETTER(Texture, filename) }
    }
};

void Texture::Generate(RNG &rng)
{

}

Schema BumpMapTextureSchema = 
{
    .parent = &TextureSchema,
    .typeName = "bitmap",
};

void BumpMapTexture::Generate(RNG &rng)
{
    Texture::Generate(rng);

    auto texture_directory = rootDir;
    texture_directory += "/resources/textures";
    std::vector<std::string> textures;
    for (const auto& texture :
        fs::directory_iterator(texture_directory))
    {
        if (std::string texStr = texture.path().u8string();
            texStr.find("_bump") != std::string::npos)
        {
            textures.push_back(texStr);
        }
    }
    if (!textures.empty())
    {
        filename = textures[rng.NextInt() % textures.size()];
    }
}