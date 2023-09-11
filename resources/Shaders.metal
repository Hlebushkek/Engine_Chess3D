#include <metal_stdlib>
#include <simd/simd.h>

typedef struct MVertex
{
    simd_packed_float3 position;
    simd_packed_float4 color;
    simd_packed_float2 texture_coord;
    simd_packed_float3 normal;
} MVertex;

typedef struct MFrameData
{
    simd::float4x4 view_matrix;
    simd::float4x4 projection_matrix;
    simd_packed_float3 camera_position;
    unsigned int framebuffer_width;
    unsigned int framebuffer_height;
} MFrameData;

typedef struct MMeshData
{
    simd::float4x4 model_matrix;
    bool is_texture_bound;
} MMeshData;

typedef struct MMaterial
{
    simd_packed_float3 ambient;
    simd_packed_float3 diffuse;
    simd_packed_float3 specular;
} MMaterial;

typedef struct MLight
{
    simd_packed_float3 ambient;
    simd_packed_float3 diffuse;
    simd_packed_float3 specular;
} MLight;

typedef enum MBufferIndex
{
    MBufferIndexMeshPositions     = 0,
    MBufferIndexMeshGenerics      = 1,
    MBufferIndexFrameData         = 2,
    MBufferIndexMeshData          = 3,
    MBufferIndexMaterialData      = 4,
    MBufferIndexLightsPosition    = 5,
    MBufferIndexLightsData        = 6,
} MBufferIndex;

typedef enum MVertexAttributes
{
    MVertexAttributePosition  = 0,
    MVertexAttributeColor     = 1,
    MVertexAttributeTexcoord  = 2,
    MVertexAttributeNormal    = 3,
} MVertexAttributes;

using namespace metal;

typedef struct
{
    float3 position  [[attribute(MVertexAttributePosition)]];
    float4 color     [[attribute(MVertexAttributeColor)]];
    float2 tex_coord [[attribute(MVertexAttributeTexcoord)]];
    float3 normal    [[attribute(MVertexAttributeNormal)]];
} Vertex;

typedef struct
{
    float4 position [[position]];
    float4 color;
    float2 tex_coord;
    float3 normal;
} ColorInOut;

vertex ColorInOut vertexMain(Vertex in [[stage_in]],
    constant MFrameData& frameData [[buffer(MBufferIndexFrameData)]],
    constant MMeshData& meshData [[buffer(MBufferIndexMeshData)]],
    uint vertexIndex [[vertex_id]])
{
    ColorInOut out;

    float4 position = float4(in.position, 1);
    position = meshData.model_matrix * position;
    position = frameData.view_matrix * position;
    position = frameData.projection_matrix * position;
    position = float4x4(
        1, 0,   0,   0,
        0, 1,   0,   0,
        0, 0, 0.5, 0.5,
        0, 0,   0,   1
    ) * position;

    out.position = position;
    out.color = in.color;
    out.tex_coord = in.tex_coord;
    out.normal = in.normal;

    return out;
}

fragment half4 fragmentMain(ColorInOut in [[ stage_in ]],
    texture2d<half, access::sample> tex [[texture(0)]],
    constant MFrameData &frameData [[buffer(MBufferIndexFrameData)]],
    constant MMeshData& meshData [[buffer(MBufferIndexMeshData)]])
{
    constexpr sampler s(address::repeat, filter::linear);
    half3 texel = tex.sample(s, in.tex_coord).rgb;

    float3 l = normalize(float3(1.0, 1.0, 0.8));
    float3 n = normalize(in.normal);
    half ndotl = half( saturate( dot( n, l ) ) );
    half4 illum = (half4(texel, 1.0) * 0.1) + (half4(texel, 1.0) * ndotl);

    // if (meshData.is_texture_bound)
        return illum;
    // else
    //     return half4(in.color);
}