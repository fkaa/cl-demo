#version 430

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct Particle {
  vec4 curr_pos;
  vec4 prev_pos;
};

layout(std430, binding = 0) buffer particles {
  Particle p[];
};

layout(location = 0) uniform vec4 att_pos;
layout(location = 1) uniform uint num_particles;

const float dt = 1. / 60.;

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main() {
  uint gid = gl_GlobalInvocationID.x;

  if (gid <= num_particles) {
    Particle particle = p[gid];
    vec4 acc, temp;

    particle.prev_pos = particle.curr_pos;

    acc = normalize(att_pos - particle.curr_pos) * 5 * length(particle.curr_pos.xy);
    temp = particle.curr_pos - particle.prev_pos + acc * dt;
    particle.curr_pos = temp;
    
    p[gid] = particle;
  }
}
