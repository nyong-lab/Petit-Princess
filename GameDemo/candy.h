/*
https://github.com/Raais/ImguiCandy

MIT License

Copyright (c) 2021 Raais N.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <stdlib.h>
#include <string>
#include "imgui.h"
#include "gradient.h"

#ifdef IMCANDYCONSTS
//Hex to ImVec4 helper: https://heximv4.rs5709.repl.co
const ImVec4 IV4_WHITE{1.000,1.000,1.000,1.000};//#ffffff
const ImVec4 IV4_BLACK{0.000,0.000,0.000,1.000};//#000000
const ImVec4 IV4_CRIMSON{0.827,0.063,0.153,1.000};//#D31027
const ImVec4 IV4_SKYBLUE{0.125,0.741,1.000,1.000};//#20BDFF
const ImVec4 IV4_AZURE{0.000,0.447,1.000,1.000};//#0072ff
const ImVec4 IV4_HOTPINK{1.000,0.000,0.600,1.000};//#FF0099
const ImVec4 IV4_CHARTREUSE{0.678,0.820,0.000,1.000};//#ADD100
const ImVec4 IV4_EMERALD{0.220,0.937,0.490,1.000};//#38ef7d
const ImVec4 IV4_BUBBLEGUM{0.988,0.404,0.980,1.000};//#fc67fa
const ImVec4 IV4_TWITCH{0.569,0.275,1.000,1.000};//#9146FF
const ImVec4 IV4_DISCORD{0.345,0.396,0.949,1.000};//#5865F2
const ImVec4 IV4_TWITTER{0.114,0.631,0.949,1.000};//#1DA1F2
const ImVec4 IV4_SPOTIFY{0.114,0.725,0.329,1.000};//#1DB954
const ImVec4 IV4_GOOGLEBLUE{0.259,0.522,0.957,1.000};//#4285F4
const ImVec4 IV4_MANGO{1.000,0.784,0.216,1.000};//#FFC837
const ImVec4 IV4_VIOLET{0.584,0.000,1.000,1.000};//#9500ff
const ImVec4 IV4_COSMICLATTE{1.000,0.973,0.906,1.000};//#fff8e7
const ImVec4 IV4_TEAL{0.314,0.788,0.765,1.000};//#50C9C3
const ImVec4 IV4_DRACULA{0.157,0.165,0.212,1.000};//#282a36
const ImVec4 IV4_2077YELLOW{0.992,0.961,0.000,1.000};//#fdf500
const ImVec4 IV4_CPNBLUE{0.000,0.878,1.000,1.0000};//#00e0ff
const ImVec4 IV4_CPNVIOLET{0.612,0.000,1.000,1.000};//#9c00ff
const ImVec4 IV4_CPNPURPLE{0.365,0.000,1.000,1.000};//#5d00ff
const ImVec4 IV4_CPNDEEP{0.000,0.039,0.122,1.000};//#000a1f
#endif

namespace ImCandy
{
      /*
      These functions are to be used inside a loop, hence the use of static variables.
      Example usage:
      ---------------------------------------------------------------
        static double s0 = 0.0;
        ImGui::PushStyleColor(ImGuiCol_Border, ImCandy::Rainbow(s0));
      ---------------------------------------------------------------

      ImVec4 Rainbow(double &static_ratio, double step = 0.01);

      ImVec4 RainbowP(double step = 0.01);

      ImVec4 Gradient2(ImVec4 col1, ImVec4 col2, double &static_ratio, double step = 0.01);

      ImVec4 Gradient3(ImVec4 col1, ImVec4 col2, ImVec4 col3, double &static_ratio, double step = 0.01);
      */

      // WARNING: High step values can cause unpleasant flashes
      ImVec4 Rainbow(double &static_ratio, double step = 0.01)
      {
          step *= 6; //just to be linear with RainbowP

          gradient::LinearColorGradient gradient;
          gradient[0.0] = {255,0,0};
          gradient[1.0] = {255,255,0};
          gradient[2.0] = {0,255,0};
          gradient[3.0] = {0,255,255};
          gradient[4.0] = {0,0,255};
          gradient[5.0] = {255,0,255};
          gradient[6.0] = {255,0,0};

          double r, g, b = 0;

          auto color = gradient(static_ratio);
          r = color[0];
          g = color[1];
          b = color[2];

          static_ratio += step;
          if (static_ratio >= 6.0)
              static_ratio = 0.0;

          return ImVec4(r / 255.0, g / 255.0, b / 255.0, 1.0);
      }

      // Portable version
      ImVec4 RainbowP(double step = 0.01)
      {
          static double static_ratio = 0.0;

          int normalized = int(static_ratio * 256 * 6);

          int region = normalized / 256;

          int x = normalized % 256;

          int r = 0, g = 0, b = 0;
          switch (region)
          {
          case 0: r = 255; g = 0;   b = 0;   g += x; break;
          case 1: r = 255; g = 255; b = 0;   r -= x; break;
          case 2: r = 0;   g = 255; b = 0;   b += x; break;
          case 3: r = 0;   g = 255; b = 255; g -= x; break;
          case 4: r = 0;   g = 0;   b = 255; r += x; break;
          case 5: r = 255; g = 0;   b = 255; b -= x; break;
          }

          static_ratio += step;
          if (static_ratio >= 1.0)
              static_ratio = 0.0;

          return ImVec4(r / 255.0, g / 255.0, b / 255.0, 1.0);
      }

      // Find some cool gradients from uigradients.com
      ImVec4 Gradient2(ImVec4 col1, ImVec4 col2, double &static_ratio, double step = 0.01)
      {
          std::valarray<double> col1a(3);
          col1a[0] = col1.x * 255.0;
          col1a[1] = col1.y * 255.0;
          col1a[2] = col1.z * 255.0;
          std::valarray<double> col2a(3);
          col2a[0] = col2.x * 255.0;
          col2a[1] = col2.y * 255.0;
          col2a[2] = col2.z * 255.0;

          gradient::LinearColorGradient gradient;
          gradient[0.0] = col1a;
          gradient[1.0] = col2a;
          gradient[2.0] = col1a;

          double r, g, b = 0;

          auto color = gradient(static_ratio);
          r = color[0];
          g = color[1];
          b = color[2];

          static_ratio += step;
          if (static_ratio >= 2.0)
              static_ratio = 0.0;

          return ImVec4(r / 255.0, g / 255.0, b / 255.0, 1.0);
      }

      // Find some cool gradients from uigradients.com
      ImVec4 Gradient3(ImVec4 col1, ImVec4 col2, ImVec4 col3, double &static_ratio, double step = 0.01)
      {
          std::valarray<double> col1a(3);
          col1a[0] = col1.x * 255.0;
          col1a[1] = col1.y * 255.0;
          col1a[2] = col1.z * 255.0;
          std::valarray<double> col2a(3);
          col2a[0] = col2.x * 255.0;
          col2a[1] = col2.y * 255.0;
          col2a[2] = col2.z * 255.0;
          std::valarray<double> col3a(3);
          col3a[0] = col3.x * 255.0;
          col3a[1] = col3.y * 255.0;
          col3a[2] = col3.z * 255.0;


          gradient::LinearColorGradient gradient;
          gradient[0.0] = col1a;
          gradient[1.0] = col2a;
          gradient[2.0] = col3a;
          gradient[3.0] = col1a;

          double r, g, b = 0;

          auto color = gradient(static_ratio);
          r = color[0];
          g = color[1];
          b = color[2];

          static_ratio += step;
          if (static_ratio >= 3.0)
              static_ratio = 0.0;

          return ImVec4(r / 255.0, g / 255.0, b / 255.0, 1.0);
      }

      /*---------------------------------------------------------------------------------------------------------
                                                        THEMES                                                 */

      void Theme_Blender(ImGuiStyle* dst = NULL)  
      {
        // 'Blender Dark' theme from v3.0.0 [Improvised]
        // Colors grabbed using X11 Soft/xcolor
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;
        ImGui::StyleColorsDark(style);//Reset to base/dark theme
        colors[ImGuiCol_Text]                   = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.19f, 0.39f, 0.69f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.20f, 0.39f, 0.69f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
        style->WindowPadding                    = ImVec2(12.00f, 8.00f);
        style->ItemSpacing                      = ImVec2(7.00f, 3.00f);
        style->GrabMinSize                      = 20.00f;
        style->WindowRounding                   = 8.00f;
        style->FrameBorderSize                  = 0.00f;
        style->FrameRounding                    = 4.00f;
        style->GrabRounding                     = 12.00f;
      }

      void Theme_Cyberpunk(ImGuiStyle* dst = NULL)  
      {
        // Cyberpunk Neon [Improvised]
        // https://github.com/Roboron3042/Cyberpunk-Neon
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;
        ImGui::StyleColorsDark(style);//Reset to base/dark theme
        colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.82f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.00f, 0.36f, 0.63f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.04f, 0.12f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.03f, 0.04f, 0.22f, 1.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.12f, 0.06f, 0.27f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.75f, 1.00f, 0.20f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.34f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.08f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.81f, 0.95f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.25f, 0.00f, 0.54f, 0.81f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f, 0.88f, 1.00f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.95f, 0.19f, 0.67f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.95f, 0.19f, 0.92f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.00f, 1.00f, 0.95f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.81f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.00f, 0.98f, 1.00f, 0.52f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.95f, 1.00f, 0.40f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.74f, 0.00f, 1.00f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.34f, 0.00f, 1.00f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.00f, 1.00f, 0.85f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.89f, 0.26f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.00f, 0.88f, 1.00f, 0.95f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.36f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.00f, 0.92f, 1.00f, 0.80f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.62f, 0.00f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.00f, 1.00f, 0.88f, 1.00f);
        style->FrameBorderSize                  = 0.00f;
        style->WindowRounding                   = 0.00f;
        style->FrameRounding                    = 0.00f;
        style->ScrollbarRounding                = 0.00f;
        style->GrabRounding                     = 0.00f;
      }
      
      void Theme_Nord(ImGuiStyle* dst = NULL)  
      {
        // Nord/Nordic GTK [Improvised]
        // https://github.com/EliverLara/Nordic
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;
        ImGui::StyleColorsDark(style);//Reset to base/dark theme
        colors[ImGuiCol_Text]                   = ImVec4(0.85f, 0.87f, 0.91f, 0.88f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.49f, 0.50f, 0.53f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.09f, 0.09f, 0.09f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.23f, 0.26f, 0.32f, 0.60f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.61f, 0.74f, 0.87f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.22f, 0.24f, 0.31f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.17f, 0.19f, 0.23f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
        style->WindowBorderSize                 = 1.00f;
        style->ChildBorderSize                  = 1.00f;
        style->PopupBorderSize                  = 1.00f;
        style->FrameBorderSize                  = 1.00f;
      }

      void Theme_MoonLight(ImGuiStyle* dst = NULL)
      {
		  // Moonlight style by deathsu/madam-herta
// https://github.com/Madam-Herta/Moonlight/
		  ImGuiStyle& style = ImGui::GetStyle();

		  style.Alpha = 1.0f;
		  style.DisabledAlpha = 1.0f;
		  style.WindowPadding = ImVec2(12.0f, 12.0f);
		  style.WindowRounding = 11.5f;
		  style.WindowBorderSize = 0.0f;
		  style.WindowMinSize = ImVec2(20.0f, 20.0f);
		  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		  style.WindowMenuButtonPosition = ImGuiDir_Right;
		  style.ChildRounding = 0.0f;
		  style.ChildBorderSize = 1.0f;
		  style.PopupRounding = 0.0f;
		  style.PopupBorderSize = 1.0f;
		  style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
		  style.FrameRounding = 11.89999961853027f;
		  style.FrameBorderSize = 0.0f;
		  style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
		  style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
		  style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
		  style.IndentSpacing = 0.0f;
		  style.ColumnsMinSpacing = 4.900000095367432f;
		  style.ScrollbarSize = 11.60000038146973f;
		  style.ScrollbarRounding = 15.89999961853027f;
		  style.GrabMinSize = 3.700000047683716f;
		  style.GrabRounding = 20.0f;
		  style.TabRounding = 0.0f;
		  style.TabBorderSize = 0.0f;
		  style.TabMinWidthForCloseButton = 0.0f;
		  style.ColorButtonPosition = ImGuiDir_Right;
		  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
		  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
		  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
		  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
		  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
		  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
		  style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
		  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
		  style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
		  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
		  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
		  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
		  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
		  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
		  style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
		  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
		  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
		  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
		  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
		  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
		  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
		  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
      }
};
