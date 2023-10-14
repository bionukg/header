// a compute shader
// input buffer
StructuredBuffer<float2> in_ : register(t0);
// omega buffer, precomputed, size is 1024
StructuredBuffer<float2> omega_ : register(t1);
// output buffer
RWStructuredBuffer<float2> out_ : register(u0);

// a 1024 point FFT
[numthreads(1024, 1, 1)]
    // use group thread id as index into buffer
    /*complex_ *fft_butterfly_v1(_Inout_ complex_ *io, _In_ uint32_t N)
    {


        complex_ up, down;



        delete[] omegaN;
        return io;
    }
    */
    void
    main(uint3 id
         : SV_DispatchThreadID)
{

/*for (uint32_t i = 0; i < N; i++)
{
    omegaN[i] = complex_(PI * i * 2 / N);
    uint32_t revi = bitrev(i, n);
    if (i < revi)
        io[i].xchg(io[revi]);
}*/

// 1024 is always 10bits, define the bit reverse function
#define n 10
#define bitrev(num, len) (reversebits(num) >> (32 - len))
// then we define the real input after bit reverse
#define in(idx) in_[bitrev(idx, 10)]
// omega should be precomputed
#define omega(idx) omega_[idx]
// we need complex add, sub, mul
#define cadd(a, b) float2(a.x + b.x, a.y + b.y)
#define csub(a, b) float2(a.x - b.x, a.y - b.y)
#define cmul(a, b) float2(a.x *b.x - a.y * b.y, a.x * b.y + a.y * b.x)

    uint delta = 1;
    uint i = 0;

        uint k = i & (delta - 1);
        uint j = (x & (~(delta - 1))) | k;
        float2 t = cmul(in(j + delta), omega(N / (2 * delta) * k));
        // io[j + delta] = io[j] - t;
        out(j + delta) = in(j)-t;
        // io[j] += t;
        out(j) = in(j) + t;

        GroupMemoryBarrierWithGroupSync();

        delta <<= 1;
        i++;

    while (i < n)
    {
        uint k = i & (delta - 1);
        uint j = (x & (~(delta - 1))) | k;
        float2 t = cmul(out(j + delta), omega(N / (2 * delta) * k));
        // io[j + delta] = io[j] - t;
        out(j + delta) = out(j)-t;
        // io[j] += t;
        out(j) = out(j) + t;

        GroupMemoryBarrierWithGroupSync();

        delta <<= 1;
        i++;
    }
}
