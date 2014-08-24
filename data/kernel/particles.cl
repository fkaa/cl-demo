#define DT 1.0f / 60.0f

__kernel void particle_run(__global float2* input, __global float2* output, const float2 att_pos) {
  unsigned int idx = get_global_id(0);
  float2 p = input[idx];

  float2 acc, temp;
  acc = normalize(att_pos - p) * 5 * length(p);
  temp = p + acc * DT;
  p = temp;

  output[idx] = p;
}
