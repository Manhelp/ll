float3 g_directDir = {-1.0f, 1.0f, -1.0f};    		
float4 g_directDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; 
float4 g_directAmbient = {0.1f, 0.1f, 0.1f, 1.0f};

float  g_pointRange;
float3 g_pointPos;
float  g_pointAtten;
float4 g_pointDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; 

static const int MAX_MATRICES = 80;
float4x3  g_boneMatrix[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4  g_matWorldViewPrj;
float4x4  g_matWorld;

struct VS_INPUT
{
    float4  pos             : POSITION;
    float3  normal          : NORMAL; 
    float4  blendWeights    : BLENDWEIGHT;
    float4  blendIndices    : BLENDINDICES;
    float3  uv0             : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  pos     : POSITION;
    float4  diffuse : COLOR;
    float2  uv0     : TEXCOORD0;
    float2  uv1     : TEXCOORD1;
};


float3 Diffuse(float3 normal,float3 lightDir)
{
    return saturate(dot(normal, lightDir));
}


VS_OUTPUT main(VS_INPUT i)
{
    VS_OUTPUT o = (VS_OUTPUT)0;
   
    //--skin pos and normal in model space
    float3  pos = 0.0f;
    float3  normal = 0.0f;    
     
    int4 indexVector = D3DCOLORtoUBYTE4(i.blendIndices);

    float blendWeightsArray[4] = (float[4])i.blendWeights;
    int   indexArray[4]        = (int[4])indexVector;
    
    pos += mul(i.pos, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
    normal += mul(i.normal, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
    pos += mul(i.pos, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
    normal += mul(i.normal, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
    pos += mul(i.pos, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
    normal += mul(i.normal, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
    pos += mul(i.pos, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
    normal += mul(i.normal, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];

    //--direct diffuse
    normal = normalize(mul(normal,g_matWorld));
    o.diffuse.xyz = g_directAmbient.xyz + Diffuse(normal,g_directDir.xyz) * g_directDiffuse.xyz;
    o.diffuse.w = g_directDiffuse.w;
        
    //--point diffuse
    o.pos=mul(float4(pos.xyz, 1.0f), g_matWorld);
    float pointDist = distance(o.pos,g_pointPos);
    if( pointDist < g_pointRange )
    {
		float pointAtten = 1 / (pointDist * g_pointAtten);
		float3 pointDir = g_pointPos - o.pos;
		o.diffuse.xyz += g_pointDiffuse.xyz * Diffuse(normal,pointDir) * pointAtten;
    }
      
    //--pos
    o.pos = mul(float4(pos.xyz, 1.0f), g_matWorldViewPrj);
   
    //--uv
    o.uv0 = i.uv0;
    o.uv1 = i.uv0;
    
    //--�ع�Ч��Ԥ�ȳ���2
    o.diffuse.rgb*=0.5f;
    
    return o;
}