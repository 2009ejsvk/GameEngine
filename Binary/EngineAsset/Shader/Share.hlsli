
// Shader에서 공용으로 사용하는 내용이 들어온다.
/*
Shader 의 레지스터
입력레지스터 : 정점정보를 넘겨받을때 사용하며 이름번호 조합으로 사용한다.
출력레지스터 : 완성된 정점정보를 넘겨줄 레지스터이다. 이 정보를 이용해서 DirectX는
화면에 출력할 위치를 잡고 색상 출력 준비를 한다.
상수레지스터 : C++코드에서 일반 데이터를 Shader로 넘겨줄 때 사용한다.
임시레지스터 : 연산 시 사용되는 레지스터
*/

// CPU -> GPU로 자주 바뀌는 값을 전달하는 기능이다.
cbuffer CBTransform : register(b0)
{
    matrix cbWorld;
    matrix cbView;
    matrix cbProj;
    matrix cbWV;
    matrix cbWVP;
};

cbuffer CBMaterial : register(b1)
{
    float4 cbBaseColor;
    float cbOpacity;
    float3 cbMaterialEmpty;
};

SamplerState sbPoint : register(s0);
SamplerState sbLinear : register(s1);
