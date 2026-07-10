/**
 *  @file r_DebugOverlay.cpp
 *
 *  @brief Implements the in-game Dear ImGui debug overlay.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#include "pch.h"
#include "r_DebugOverlay.h"

#include <imgui.h>

namespace FuturaLibrary
{
	void DebugOverlay::Draw(DebugOverlayState& state, const DebugOverlayFrameData& frameData)
	{
		if (!state.ShowStats)
			return;

		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
		ImGui::SetNextWindowPos(ImVec2(12.0f, 12.0f), ImGuiCond_FirstUseEver);

		ImGui::Begin("Debug Stats", nullptr, flags);

		ImGui::SeparatorText("Frame");
		ImGui::Text("FPS: %.0f", frameData.FPS);
		ImGui::Text("Frame Time: %.2f ms", frameData.FrameTimeMs);

		ImGui::SeparatorText("Renderer");
		ImGui::Text("Draw Calls: %u", frameData.Render.DrawCalls);
		ImGui::Text("Submitted Meshes: %u", frameData.Render.SubmittedMeshes);
		ImGui::Text("Triangles: %u", frameData.Render.Triangles);
		ImGui::Text("Surfaces: %u visible / %u total", frameData.Render.VisibleSurfaces, frameData.Render.TotalSurfaces);
		ImGui::Text("Culled Surfaces: %u", frameData.Render.CulledSurfaces);

		ImGui::SeparatorText("World Acceleration");
		ImGui::Text("Grid Cell Size: %.2f", frameData.Acceleration.CellSize);
		ImGui::Text("Occupied Cells: %u", frameData.Acceleration.OccupiedCells);
		ImGui::Text("Indexed Surfaces: %u", frameData.Acceleration.IndexedSurfaces);
		ImGui::Text("Indexed Triangles: %u", frameData.Acceleration.IndexedTriangles);

		ImGui::SeparatorText("Debug Draw");
		ImGui::Text("Lines: %u", frameData.DebugDraw.LineCount);
		ImGui::Text("Vertices: %u", frameData.DebugDraw.VertexCount);
		ImGui::Text("Flushes: %u", frameData.DebugDraw.FlushCount);

		ImGui::SeparatorText("Physics");
		ImGui::Text("Collision Time: %.3f ms", frameData.Collision.CollisionTimeMs);
		ImGui::Text("Broad-Phase Queries: %u", frameData.Collision.BroadPhaseQueries);
		ImGui::Text("Candidate Surfaces: %u", frameData.Collision.CandidateSurfaces);
		ImGui::Text("Candidate Triangles: %u", frameData.Collision.CandidateTriangles);
		ImGui::Text("Narrow-Phase Tests: %u", frameData.Collision.NarrowPhaseTests);
		ImGui::Text("Contacts Generated: %u", frameData.Collision.ContactsGenerated);

		ImGui::SeparatorText("World Debug");
		ImGui::Checkbox("Bounds (F1)", &state.DrawSettings.DrawBounds);

		ImGui::End();
	}
}
