#include "Texture.h"

#include "stb_image/stb_image.h"

#define CHANNEL_REQUESTED 4

Texture::Texture(const std::string& path) 
	: m_RendererID(0), m_FIlePath(path), m_localBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	bool success = load_image(image, path, m_Width, m_Height);
	if (!success)
	{
		std::cout << "Error loading image\n";
		return;
	}

	//m_localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, CHANNEL_REQUESTED);
}

Texture::~Texture() 
{
	if (m_localBuffer) {
		stbi_image_free(m_localBuffer);
	}
}

bool Texture::load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y)
{
	int n;
	unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, CHANNEL_REQUESTED);
	if (data != nullptr)
	{
		image = std::vector<unsigned char>(data, data + x * y * CHANNEL_REQUESTED);
	}
	stbi_image_free(data);
	return (data != nullptr);
}

Color Texture::Sample(const Vector3& uv) const
{
	int i = uv.x * GetWidth();
	int j = uv.y * GetHeight();

	if (uv.x >= 1.0f) {
		return Color();
	}

	if (uv.x < 0.0f) {
		return Color();
	}

	if (uv.y < 0.0f) {
		return Color();
	}

	if (uv.y >= 1.0f) {
		return Color();
	}

	const size_t RGBA = CHANNEL_REQUESTED;
	size_t index = RGBA * (j * m_Width + i);

	unsigned char r = image[index + 0];
	unsigned char g = image[index + 1];
	unsigned char b = image[index + 2];
	unsigned char a = image[index + 3];

	//std::cout << (int)r << " " << (int)g << " " << (int)b << " " << (int)a << std::endl;
	return Color((int)r, (int)g, (int)b);
}
