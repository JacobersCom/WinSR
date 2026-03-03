
struct PSout
{
	float4 color : SV_TARGET;
};

PSout main()
{
    PSout o;
    o.color = float4(1.0, 1.0, 1.0, 1.0);
    return o;
}