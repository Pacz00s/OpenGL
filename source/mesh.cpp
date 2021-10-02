#include "../header/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"

Mesh::Mesh(std::vector<Vertex3dUVNormal> vertices, std::vector<unsigned short> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	// stworznei kszta�tu w buforach

	// ustaeienie wierzcho�k�w
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ustawienie kraw�dzi
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(std::string filePath)
{
    // �adowanie modelu je�li istnieje
    std::ifstream file(filePath);

    if (!file.good())
    {
        std::cout << "Nie znaleziono modelu: " << filePath << std::endl;
        return;
    }

	// zmienne assimp
	Assimp::Importer importer;
	const aiScene* scene = NULL;
	unsigned int n = 0, t;

	// �adowanie pliku do sceny
	for(int k = 0; k < 1; k++)
	scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality | aiProcess_PreTransformVertices);

	// �adowanie tylko pierwszego mesha z pliku
	const struct aiMesh* mesh = scene->mMeshes[0];

	// pojemnik na wierzcho�ki
	for (t = 0; t < mesh->mNumVertices; ++t)
	{
		// stworzneie wierzcho�ka
		Vertex3dUVNormal v;

		// skopiowanie po�o�enie, normalnych i tektury wierzcho�ka z modelu
		// nie ma dbania o wyciek pami�ci, jezeli co� jest nie tak, sprawd� plik z modelem
		memcpy(&v.m_position, 	&mesh->mVertices[t], 		sizeof(glm::vec3));
		memcpy(&v.m_normal, 	&mesh->mNormals[t], 		sizeof(glm::vec3));
		memcpy(&v.m_texCoord, 	&mesh->mTextureCoords[0][t],sizeof(glm::vec2));

		// dodanie wierzcho�ka do bufora
		m_vertices.push_back(v);
	}

	// stworzenie buforu �cian
	for (t = 0; t < mesh->mNumFaces; ++t)
	{
		const struct aiFace* face = &mesh->mFaces[t];

		 // dodanie 3 wierzcho�k�w jako �ciana o kszta�cie tr�jk�ta
		 for (int i = 0; i < 3; i++)
		 {
			 m_indices.push_back(face->mIndices[i]);
		 }

		 // kiedy mamy do czynienia z prostok�tem, dodaje do tr�j�ta kolejne 3 wierzcho�ki
		 if (face->mNumIndices == 4)
		 {
			 m_indices.push_back(face->mIndices[0]);
			 m_indices.push_back(face->mIndices[2]);
			 m_indices.push_back(face->mIndices[3]);
		 }
	}

	printf("Za�adowano %d wierzcho�k�w, %d kraw�dzi \n", (int)m_vertices.size(), (int)m_indices.size());

	// bufory wierzcho�k�w
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	// modyfikujemy raz, u�ywamy ci�gle dlatego static
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // kraw�dzie
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	// usuwanie bufor�w kraw�dzi i wierzcho�k�w
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}

// makro czyni�ce u�ycie funkcji czytelniejszym, patrz Draw()
// ustawia tablice w�ano�ci generowanych wierzcho�k�w
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element));

void Mesh::Draw()
{
	// Do��czanie krawedzi i wierzhco�k�w mesha
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Ustawianie w�asno�ci, przypisanie z modelu do "generycznych"
	SetupAttribute(0, 3, GL_FLOAT, Vertex3dUVNormal, m_position);
	SetupAttribute(1, 2, GL_FLOAT, Vertex3dUVNormal, m_texCoord);
	SetupAttribute(2, 3, GL_FLOAT, Vertex3dUVNormal, m_normal);

	// w��czenie pozycji, textury, materia�u
	for (int i = 0; i < 3; i++)
		glEnableVertexAttribArray(i);

	// rysowanie wszystkich kraw�dzi
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	// od��czenie bufora kraw�dzi i wierzcho�k�w po narysowaniu
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// wy��czenie poyzcji, textury, materia�u.
	for (int i = 0; i < 3; i++)
		glDisableVertexAttribArray(i);

}
