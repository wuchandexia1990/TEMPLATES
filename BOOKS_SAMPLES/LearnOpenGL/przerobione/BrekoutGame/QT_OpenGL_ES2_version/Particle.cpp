//
//  Particle.cpp
//  OpenGLBreakout
//
//  Created by 梅宇宸 on 16/12/26.
//  Copyright © 2016年 梅宇宸. All rights reserved.
//

#include "Particle.hpp"
#include <iostream>
using namespace std;

ParticleGenerator::ParticleGenerator (Shader shader, Texture2D texture, GLuint amount)
    : shader (shader), texture (texture), amount (amount)
{
    this->init ();
}

static bool draw_vao = true;

void ParticleGenerator::init ()
{
    // Set up mesh and attribute properties

    GLfloat particle_quad[] = {
        // Pos      // TexCoord
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    if(draw_vao){
        glGenVertexArrays (1, &this->VAO);
        glGenBuffers (1, &this->VBO);
        glBindVertexArray (this->VAO);

        // Fill mesh buffer
        glBindBuffer (GL_ARRAY_BUFFER, this->VBO);
        glBufferData (GL_ARRAY_BUFFER, sizeof (particle_quad), particle_quad, GL_STATIC_DRAW);

        // Set mesh attributes
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glBindVertexArray (0);
    }else{
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData (GL_ARRAY_BUFFER, sizeof (particle_quad), particle_quad, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    // Create this->amount default particle instances
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back (Particle ());
}

void ParticleGenerator::Update (GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset)
{
    // Add new particles
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle ();
        this->respawnParticle (this->particles[unusedParticle], object, offset);
    }
    
    // Update all particles
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5;
        }
    }
}

// Render all particles
void ParticleGenerator::Draw ()
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    this->shader.Use ();
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.SetVector2f ("offset", particle.Position);
            this->shader.SetVector4f ("color", particle.Color);




            if(draw_vao){


                glBindVertexArray (this->VAO);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->texture.ID);
                glUniform1i(glGetUniformLocation (this->shader.ID, "sprite" ), GL_TEXTURE0);

                glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray (0);
            }else{

                glBindBuffer(GL_ARRAY_BUFFER, this->VBO);


                glVertexAttribPointer (glGetAttribLocation(this->shader.ID,"vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray (glGetAttribLocation(this->shader.ID,"vertex"));


                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->texture.ID);
                glUniform1i(glGetUniformLocation (this->shader.ID, "sprite" ), GL_TEXTURE0);


                glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

            }
        }
    }
    // Don't forget to reset to default blending mode
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle ()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle (Particle &particle, GameObject &object, glm::vec2 offset)
{
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}