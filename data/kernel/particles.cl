#define DT 1.0f / 60.0f

__kernel void update(__global float2* positions, __global float2* velocities, const float2 att_pos) {
  unsigned int idx = get_global_id(0);
  
  float2 position = positions[idx];
  float2 velocity = velocities[idx];

  velocity += normalize(att_pos - position) * 2 * DT;
  position += velocity;

  positions[idx] = position;
  velocities[idx] = velocity;
}
