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
	// stworznei kszta³tu w buforach

	// ustaeienie wierzcho³ków
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ustawienie krawêdzi
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(std::string filePath)
{
    // ³adowanie modelu jeœli istnieje
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

	// ³adowanie pliku do sceny
	for(int k = 0; k < 1; k++)
	scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality | aiProcess_PreTransformVertices);

	// ³adowanie tylko pierwszego mesha z pliku
	const struct aiMesh* mesh = scene->mMeshes[0];

	// pojemnik na wierzcho³ki
	for (t = 0; t < mesh->mNumVertices; ++t)
	{
		// stworzneie wierzcho³ka
		Vertex3dUVNormal v;

		// skopiowanie po³o¿enie, normalnych i tektury wierzcho³ka z modelu
		// nie ma dbania o wyciek pamiêci, jezeli coœ jest nie tak, sprawdŸ plik z modelem
		memcpy(&v.m_position, 	&mesh->mVertices[t], 		sizeof(glm::vec3));
		memcpy(&v.m_normal, 	&mesh->mNormals[t], 		sizeof(glm::vec3));
		memcpy(&v.m_texCoord, 	&mesh->mTextureCoords[0][t],sizeof(glm::vec2));

		// dodanie wierzcho³ka do bufora
		m_vertices.push_back(v);
	}

	// stworzenie buforu œcian
	for (t = 0; t < mesh->mNumFaces; ++t)
	{
		const struct aiFace* face = &mesh->mFaces[t];

		 // dodanie 3 wierzcho³ków jako œciana o kszta³cie trójk¹ta
		 for (int i = 0; i < 3; i++)
		 {
			 m_indices.push_back(face->mIndices[i]);
		 }

		 // kiedy mamy do czynienia z prostok¹tem, dodaje do trój¹ta kolejne 3 wierzcho³ki
		 if (face->mNumIndices == 4)
		 {
			 m_indices.push_back(face->mIndices[0]);
			 m_indices.push_back(face->mIndices[2]);
			 m_indices.push_back(face->mIndices[3]);
		 }
	}

	printf("Za³adowano %d wierzcho³ków, %d krawêdzi \n", (int)m_vertices.size(), (int)m_indices.size());

	// bufory wierzcho³ków
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	// modyfikujemy raz, u¿ywamy ci¹gle dlatego static
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // krawêdzie
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	// usuwanie buforów krawêdzi i wierzcho³ków
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}

// makro czyni¹ce u¿ycie funkcji czytelniejszym, patrz Draw()
// ustawia tablice w³anoœci generowanych wierzcho³ków
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element));

void Mesh::Draw()
{
	// Do³¹czanie krawedzi i wierzhco³ków mesha
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Ustawianie w³asnoœci, przypisanie z modelu do "generycznych"
	SetupAttribute(0, 3, GL_FLOAT, Vertex3dUVNormal, m_position);
	SetupAttribute(1, 2, GL_FLOAT, Vertex3dUVNormal, m_texCoord);
	SetupAttribute(2, 3, GL_FLOAT, Vertex3dUVNormal, m_normal);

	// w³¹czenie pozycji, textury, materia³u
	for (int i = 0; i < 3; i++)
		glEnableVertexAttribArray(i);

	// rysowanie wszystkich krawêdzi
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	// od³¹czenie bufora krawêdzi i wierzcho³ków po narysowaniu
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// wy³¹czenie poyzcji, textury, materia³u.
	for (int i = 0; i < 3; i++)
		glDisableVertexAttribArray(i);

}
