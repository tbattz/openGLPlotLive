/*
 * fonts.h
 *
 *  Created on: 5Jan.,2017
 *      Author: bcub3d-desktop
 */

#ifndef P_FONTS_H_
#define P_FONTS_H_

// Free Type Library
#include <ft2build.h>
#include FT_FREETYPE_H

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext.hpp"

// Standard Includes
#include <map>
#include <vector>

// Project Includes
#include "shader.h"

namespace GLPL {
	// Character Structure
	struct Character2 {
		GLuint		TextureID;	// ID handle of the glyph texture
		glm::ivec2	Size;		// size of glyph
		glm::ivec2	Bearing;	// Offset from baseline to left/top of glyph
		GLuint		Advance;	// Offset to advance to next glyph
	};

	class GLFont {
	public:
		std::map<GLchar, Character2> Characters;
		GLuint VAO, VBO;

		GLFont(const GLchar* fontPath) {
			/* Initialises a font. */

			// Enable Blending for transparent text background
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Initialise Font
			FT_Library ft;
			if(FT_Init_FreeType(&ft)) {
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << '\n';
			}
			FT_Face face;
			if(FT_New_Face(ft,fontPath,0,&face)) {
				std::cout << "ERROR::FREETYPE: Failed to load font" << '\n';
			}
			// Set Font Size
			FT_Set_Pixel_Sizes(face,0,48);

			// Load Glyphs
			glPixelStoref(GL_UNPACK_ALIGNMENT,1); // Disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);

			// Load first 128 Glyphs
			for(GLubyte c = 0; c < 128; c++) {
				// Load Single glyph
				if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					std::cout << "ERROR:FREETYPE: Failed to load Glyph" << '\n';
					continue;
				}
				// Generate Texture
				GLuint texture;
				glGenTextures(1,&texture);
				glBindTexture(GL_TEXTURE_2D,texture);
				glTexImage2D(
						GL_TEXTURE_2D,0,GL_RED,face->glyph->bitmap.width,face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);
				// Set Texture Options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// Store characters
                Character2 character = {texture,glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
										glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
										static_cast<unsigned int>(face->glyph->advance.x)};
				this->Characters.insert(std::pair<GLchar, Character2>(c,character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);

			// Clear Resources
			FT_Done_Face(face);
			FT_Done_FreeType(ft);

			// Configure VAO and VBO for quads
			glGenVertexArrays(1,&this->VAO);
			glGenBuffers(1,&this->VBO);
			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
			glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4,NULL,GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),0);
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindVertexArray(0);
		}

		void RenderText(Shader* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int align) {
			/* Renders text to the screen in a given location. */
			// align: 0 - Left Top Align
			// align: 1 - Left Bottom Align
			// align: 2	- Horizontal Centre Align
			// align: 3 - Vertical Centre Align
			// Activate correct shader
			shader->Use();
			glUniform3f(glGetUniformLocation(shader->Program, "textColor"),color.x, color.y, color.z);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(this->VAO);

			// Iterate through each character
			if(align==0) {
				drawLeftTopAligned(text, x, y, scale);
			} else if (align==1) {
				drawLeftBottomAligned(text, x, y, scale);
			} else if (align==2) {
				drawHCentreAligned(text, x, y, scale);
			} else if (align==3) {
				drawVCentreAligned(text, x, y, scale);
			}

			// Unbind Arrays
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D,0);
		}

		void drawLeftTopAligned(std::string text, GLfloat x, GLfloat y, GLfloat scale) {
			// Draw text as left top aligned
			std::string::const_iterator c;
			bool firstRow = false;
			float ystart = 0;
			float ymax = 0;
			float yoffset = 0;
			float xstart = 0;

			xstart = this->Characters[0].Bearing.x;

			// Get Offsets Height
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Find y starting position
					if(!firstRow) {
						firstRow = true;
						ystart = ymax;
						ymax = 0;
					}
					ymax = 0;
				} else {
					// Get maximum y
					GLfloat h = ch.Size.y * scale;
					if(h > ymax) {
						ymax = h;
					}
				}
			}

			// Reset ymax
			ymax = 0;

			// Draw glyphs
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Calculate offset
					yoffset += ymax*1.25;
					ymax = 0;
					x = xstart;
				} else {
					GLfloat xpos = x + ch.Bearing.x * scale;
					GLfloat ypos = y - ((ch.Size.y - ch.Bearing.y) * scale) - yoffset - ystart; // Accounts for g and p offsets

					GLfloat w = ch.Size.x * scale;
					GLfloat h = ch.Size.y * scale;

					if(h > ymax) {
						ymax = h;
					}

					// Update the VBO for each character
					GLfloat vertices[6][4] = {
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos,		ypos,		0.0, 1.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos + w,	ypos + h,	1.0, 0.0},
					};
					// Render glyph texture over quad face
					glBindTexture(GL_TEXTURE_2D, ch.TextureID);
					// Update VBO memory
					glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
					glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER,0);
					// Render quad face
					glDrawArrays(GL_TRIANGLES,0,6);
					// Advance pos to next glyph (advance numb is 1/64 pixels)
					x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 6)
				}
			}
		}

		void drawLeftBottomAligned(std::string text, GLfloat x, GLfloat y, GLfloat scale) {
			// Draw text as left bottom aligned
			std::string::const_iterator c;
			bool firstRow = false;
			float ystart = 0;
			float ymax = 0;
			float yoffset = 0;
			float xstart = 0;

			xstart = this->Characters[0].Bearing.x;

			// Get Offsets Height
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Find y starting position
					if(!firstRow) {
						firstRow = true;
					}
					ystart += ymax;
					ymax = 0;
				} else {
					// Get maximum y
					GLfloat h = ch.Size.y * scale;
					if(h > ymax) {
						ymax = h;
					}
				}
			}

			// Reset ymax
			ymax = 0;

			// Draw glyphs
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Calculate offset
					yoffset += ymax*1.25;
					ymax = 0;
					x = xstart;
				} else {
					GLfloat xpos = x + ch.Bearing.x * scale;
					GLfloat ypos = y - ((ch.Size.y - ch.Bearing.y) * scale) - yoffset + ystart; // Accounts for g and p offsets

					GLfloat w = ch.Size.x * scale;
					GLfloat h = ch.Size.y * scale;

					if(h > ymax) {
						ymax = h;
					}

					// Update the VBO for each character
					GLfloat vertices[6][4] = {
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos,		ypos,		0.0, 1.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos + w,	ypos + h,	1.0, 0.0},
					};
					// Render glyph texture over quad face
					glBindTexture(GL_TEXTURE_2D, ch.TextureID);
					// Update VBO memory
					glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
					glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER,0);
					// Render quad face
					glDrawArrays(GL_TRIANGLES,0,6);
					// Advance pos to next glyph (advance numb is 1/64 pixels)
					x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 6)
				}
			}
		}

		void drawHCentreAligned(std::string text, GLfloat x, GLfloat y, GLfloat scale) {
			// Draw text as horizontal centre aligned
			std::string::const_iterator c;
			bool firstRow = false;
			float ymax = 0;
			float ystart = 0;
			float yoffset = 0;
			float xoffset = 0;
			float currWidth = 0;
            std::vector<float> lineWidth;
			float xstart = 0;

			xstart = -this->Characters[0].Bearing.x;

			// Get Offsets Height
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Find y starting position
					if(!firstRow) {
						firstRow = true;
						ystart = ymax;
						ymax = 0;
						lineWidth.push_back(currWidth);
						currWidth = 0;
					}
					ymax = 0;
				} else {
					// Get maximum y
					GLfloat h = ch.Size.y * scale;
					if(h > ymax) {
						ymax = h;
					}
					// Increment width
					currWidth += (ch.Size.x + ch.Bearing.x) * scale;
				}
			}

			// Reset ymax
			ymax = 0;
			// Set starting x offset
			lineWidth.push_back(currWidth);
			xoffset = lineWidth[0]/2.0;

			// Draw glyphs
			int i=0;
			for(c=text.begin(); c != text.end(); c++) {
                Character2 ch = this->Characters[*c];
				if(*c=='\n') {
					// Increment row
					i += 1;
					xoffset = lineWidth[i]/2.0;
					// Calculate offset
					yoffset += ymax*1.25;
					ymax = 0;
					x = xstart;
				} else {
					GLfloat xpos = x - xoffset + (ch.Bearing.x * scale) + xstart;
					GLfloat ypos = y - ((ch.Size.y - ch.Bearing.y) * scale) - yoffset - ystart; // Accounts for g and p offsets

					GLfloat w = ch.Size.x * scale;
					GLfloat h = ch.Size.y * scale;

					if(h > ymax) {
						ymax = h;
					}

					// Update the VBO for each character
					GLfloat vertices[6][4] = {
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos,		ypos,		0.0, 1.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos,		ypos + h,	0.0, 0.0},
							{xpos + w,	ypos,		1.0, 1.0},
							{xpos + w,	ypos + h,	1.0, 0.0},
					};
					// Render glyph texture over quad face
					glBindTexture(GL_TEXTURE_2D, ch.TextureID);
					// Update VBO memory
					glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
					glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER,0);
					// Render quad face
					glDrawArrays(GL_TRIANGLES,0,6);
					// Advance pos to next glyph (advance numb is 1/64 pixels)
					x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 6)
				}
			}
		}

		void drawVCentreAligned(std::string text, GLfloat x, GLfloat y, GLfloat scale) {
				// Draw text as vertical centre aligned
				std::string::const_iterator c;
				bool firstRow = false;
				float ymax = 0;
				float ystart = 0;
				float yoffset = 0;
				float xoffset = 0;
				float currWidth = 0;
                std::vector<float> lineWidth;
				float xstart = 0;

				xstart = -this->Characters[0].Bearing.x;

				// Get Offsets Height
				for(c=text.begin(); c != text.end(); c++) {
                    Character2 ch = this->Characters[*c];
					if(*c=='\n') {
						// Find y starting position
						if(!firstRow) {
							firstRow = true;
							ystart = ymax;
							ymax = 0;
							lineWidth.push_back(currWidth);
							currWidth = 0;
						}
						ymax = 0;
					} else {
						// Get maximum y
						GLfloat h = ch.Size.y * scale;
						if(h > ymax) {
							ymax = h;
						}
						// Increment width
						currWidth += (ch.Size.x + ch.Bearing.x) * scale;
					}
				}

				// Reset ymax
				ymax = 0;
				// Set starting x offset
				lineWidth.push_back(currWidth);
				xoffset = lineWidth[0]/2.0;

				// Draw glyphs
				int i=0;
				for(c=text.begin(); c != text.end(); c++) {
                    Character2 ch = this->Characters[*c];
					if(*c=='\n') {
						// Increment row
						i += 1;
						xoffset = lineWidth[i]/2.0;
						// Calculate offset
						yoffset += ymax*1.25;
						ymax = 0;
						x = xstart;
					} else {
						GLfloat xpos = x - xoffset + (ch.Bearing.x * scale) + xstart;
						GLfloat ypos = y - ((ch.Size.y - ch.Bearing.y) * scale) - yoffset - ystart; // Accounts for g and p offsets

						GLfloat w = ch.Size.x * scale;
						GLfloat h = ch.Size.y * scale;

						// Adjust y position
						ypos -= h/2.0;

						if(h > ymax) {
							ymax = h;
						}

						// Update the VBO for each character
						GLfloat vertices[6][4] = {
								{xpos,		ypos + h,	0.0, 0.0},
								{xpos,		ypos,		0.0, 1.0},
								{xpos + w,	ypos,		1.0, 1.0},
								{xpos,		ypos + h,	0.0, 0.0},
								{xpos + w,	ypos,		1.0, 1.0},
								{xpos + w,	ypos + h,	1.0, 0.0},
						};
						// Render glyph texture over quad face
						glBindTexture(GL_TEXTURE_2D, ch.TextureID);
						// Update VBO memory
						glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
						glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_DYNAMIC_DRAW);
						glBindBuffer(GL_ARRAY_BUFFER,0);
						// Render quad face
						glDrawArrays(GL_TRIANGLES,0,6);
						// Advance pos to next glyph (advance numb is 1/64 pixels)
						x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 6)
					}
				}
			}



	private:
	};

}

#endif /* P_FONTS_H_ */
