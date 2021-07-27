#pragma once

#include "Renderer.h"
#include "Ray Tracing Lib/Vector3.h"
#include "Ray Tracing Lib/Color.h"

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FIlePath;
	std::vector<unsigned char> image;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture();
	Texture(const std::string& path);
	~Texture();
	bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);
	void load_image(const std::string& path);
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	Color Sample(const Vector3& uv) const;
};