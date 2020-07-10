#include "Model.h"
void Model::load(const std::string& filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            glm::fvec3 v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            vertices.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            glm::fvec3 n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            normals.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            glm::fvec2 uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            texCoords.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<glm::ivec3> f;
            glm::ivec3 tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
    std::cerr << "# v# " << vertices.size() << " f# " << faces.size() << " vt# " << texCoords.size() << " vn# " << normals.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
    // load_texture(filename, "_nm_tangent.tga", normalmap_);
}

void Model::load_texture(const std::string& filename, const char* suffix, TGAImage& img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(glm::fvec2 uvf) {
    glm::ivec2 uv(uvf[0] * diffusemap_.get_width(), uvf[1] * diffusemap_.get_height());
    return diffusemap_.get(uv[0], uv[1]);
}

glm::fvec3 Model::normal(glm::fvec3 uvf) {
    glm::ivec2 uv(uvf[0] * normalmap_.get_width(), uvf[1] * normalmap_.get_height());
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    glm::fvec3 res;
    for (int i = 0; i < 3; i++) {
        res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f;
    }
    return res;
}
