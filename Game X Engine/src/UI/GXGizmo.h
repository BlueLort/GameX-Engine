#pragma once
#include <ImGuizmo/ImGuizmo.h>
#include "Maths/GXMaths.h"
namespace gx {
	enum GXGizmoOperation
	{
		GX_TRANSLATE = ImGuizmo::OPERATION::TRANSLATE,
		GX_ROTATE = ImGuizmo::OPERATION::ROTATE,
		GX_SCALE = ImGuizmo::OPERATION::SCALE,
		GX_BOUNDS = ImGuizmo::OPERATION::BOUNDS
	};

	enum GXGizmoMode
	{
		GX_LOCAL = ImGuizmo::MODE::LOCAL,
		GX_WORLD = ImGuizmo::MODE::WORLD
	};
	class GX_DLL GXGizmo
	{
	public:
		GXGizmo() = delete;
		inline static void begin() { ImGuizmo::BeginFrame(); }
		inline static GXBool isOver() { return ImGuizmo::IsOver(); }
		inline static GXBool isUsing() { return ImGuizmo::IsUsing(); }
		inline static void setEnabled(GXBool enabled) { ImGuizmo::Enable(enabled); }
		inline static void setOrthographic(GXBool isOrthographic) { ImGuizmo::SetOrthographic(isOrthographic); }
		inline static void setRect(GXFloat x, GXFloat y, GXFloat width, GXFloat height) {
			ImGuizmo::SetRect(x, y, width, height);
		}
		void static manipulate(const GXFloat* view, const GXFloat* projection, GXGizmoOperation operation, GXGizmoMode mode, GXFloat* matrix, GXFloat* deltaMatrix = nullptr, GXFloat* snap = nullptr, GXFloat* localBounds = nullptr, GXFloat* boundsSnap = nullptr) {
			ImGuizmo::SetDrawlist();
			ImGuizmo::Manipulate(view, projection, static_cast<ImGuizmo::OPERATION>(operation), static_cast<ImGuizmo::MODE>(mode), matrix, deltaMatrix, snap, localBounds, boundsSnap);
		}
		void static viewManipulate(GXFloat* view, GXFloat length, GXVec2 position, GXVec2 size, GXuint32 backgroundColor) {
			ImGuizmo::ViewManipulate(view, length, ImVec2(position.x, position.y), ImVec2(size.x, size.y), backgroundColor);
		}
		//Helpers using Imguizmo helpers for now
		inline static void decomposeMatrix(const GXFloat* matrix , GXFloat* position, GXFloat* rotation, GXFloat* scale) {
			ImGuizmo::DecomposeMatrixToComponents(matrix, position, rotation, scale);
		}
		inline static void recomposeMatrix(const GXFloat* position, const GXFloat* rotation, const GXFloat* scale,GXFloat* matrix) {
			ImGuizmo::RecomposeMatrixFromComponents(position, rotation, scale, matrix);
		}
	private:

	};
}