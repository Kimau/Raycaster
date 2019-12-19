#pragma once

typedef unsigned char u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef char s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;

struct quat;
struct vec2;
struct vec3;
struct vec4;
struct ray;

typedef vec2 CTexCoord;
typedef vec4 CSphere;

typedef vec3 CVert;
typedef vec3 CNorm;

#define PI 3.141592654f // Simpler PI
#define DEG_TO_RAD 0.017453292519943f
#define RAD_TO_DEG 57.295779513f
