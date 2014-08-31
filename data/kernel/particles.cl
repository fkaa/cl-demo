#define DT 1.0f / 60.0f

float2 attract(float2 a, float2 b) {
  float2 v = b - a;
  float r2 = dot(v, v);
  r2 += 0.01f;
  float inv_dist = 1.f / sqrt(r2);
  float inv_dist_cubed = inv_dist;
  return v * inv_dist_cubed;
}

__kernel void update(__global float2* positions, __global float2* velocities, const float2 att_pos, const float drag_factor) {
  unsigned int idx = get_global_id(0);
  
  float2 position = positions[idx];
  float2 velocity = velocities[idx];

  //velocity += drag_factor * (normalize(att_pos - position) * DT);
  velocity += attract(position, att_pos) * 2.5 * drag_factor;
  position += velocity;
  velocity *= 0.999f;

  positions[idx] = position;
  velocities[idx] = velocity;
}
 
