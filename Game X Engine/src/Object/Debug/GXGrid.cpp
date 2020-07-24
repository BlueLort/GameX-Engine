#include "pch.h"
#include "GXGrid.h"

namespace gx {
	constexpr GXFloat HORIZONTAL_SPACING = 1.0f;
	constexpr GXFloat VERTICAL_SPACING = 1.0f;
	constexpr GXuint32 N_HORIZONTAL = 150;
	constexpr GXuint32 N_VERTICAL = 150;
	GXuint32 GXGrid::createGrid(std::vector<Vertex3D>& gridVerts) {
		
		gridVerts.reserve(N_HORIZONTAL * N_VERTICAL);
		GXFloat halfSpacingVertical = (N_VERTICAL / 2.0f) * VERTICAL_SPACING; // used to center grid
		GXFloat halfSpacingHorizontal = (N_HORIZONTAL / 2.0f) * HORIZONTAL_SPACING;// used to center grid
		GXFloat xLoc, zLoc;
		for (GXint32 i = 0; i < N_VERTICAL; i++) {
			zLoc = i * VERTICAL_SPACING - halfSpacingVertical;
			for (GXint32 j = 0; j < N_HORIZONTAL; j++) {
				xLoc = j * HORIZONTAL_SPACING - halfSpacingHorizontal;
				gridVerts.emplace_back(GXVec3(xLoc, 0.0f, zLoc));
			}
		}
		GXint32 k = 0;
		GXuint32 nQuadsVertical = N_VERTICAL - 1;
		GXuint32 nQuadsHorizontal = N_HORIZONTAL - 1;
		GXuint32 nIndices = nQuadsVertical * nQuadsHorizontal * 8;
		indices = new GXuint32[nIndices];
		for (GXint32 z = 0; z < nQuadsVertical; z++)
		{
			for (GXint32 x = 0; x < nQuadsHorizontal; x++)
			{
				indices[k] = z * N_VERTICAL + x;
				indices[k + 1] = (z + 1) * N_VERTICAL + x;

				indices[k + 2] = (z + 1) * N_VERTICAL + x;
				indices[k + 3] = (z + 1) * N_VERTICAL + (x + 1);

				indices[k + 4] = (z + 1) * N_VERTICAL + (x + 1);
				indices[k + 5] = z * N_VERTICAL + (x + 1);

				indices[k + 6] = z * N_VERTICAL + (x + 1);
				indices[k + 7] = z * N_VERTICAL + x;
				k += 8; // next quad
			}
		}
		return nIndices;
	}
	void GXGrid::init(const char* shaderPath) {

		if (shaderPath == nullptr) {
			shader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_GRID);
		}
		else {
			shader = GXShaderManager::getShader(shaderPath);
		}
		//SET UP THE GRID
		std::vector<Vertex3D> verts;
		GXuint32 nIndices = createGrid(verts);
		//SET UP THE BUFFER
		std::shared_ptr<GXGraphicsBufferManager> GBM;
		GBM.reset(new GXGraphicsBufferManager());
		GBM->initFull(&verts[0], sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
		GBM->uploadIndicesToBuffer(&indices[0], nIndices * sizeof(GXuint32), nIndices);
		GBM->setAttribPointer(0, 3, GX_FLOAT, offsetof(Vertex3D, position));
		GBM->endStream();
		components.push_back(std::make_shared<GXMeshComponent>(this->GXID,GBM));
		GXGraphicsContext::setLineWidth(2.0f);
	}

	void GXGrid::update(float deltaTime) {
		for (auto& component : components) {
			component->update(deltaTime);
		}
	}

	void GXGrid::draw()
	{
		//For rendering as rendering happens in GXMeshComponent
		this->shader->use();
		this->shader->setMat4("model", GXMat4(1.0f));//identity
		this->shader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
		for (auto& component : components) {
			component->draw(shader, RenderType::GX_LINES, false);
		}
	}

	void GXGrid::destroy() {
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}
}