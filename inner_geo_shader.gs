#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

// 传递给片段着色器的输出
out vec3 fragPos;
out vec3 normal;
out vec2 TexCoords; 

uniform float time;
uniform bool is_collide;
uniform float collision_time;

vec3 physical_simulation(float time, vec3 normal) 
{
    float gravity = 9.8; // 重力加速度（单位时间内速度的增加量）
    float discount_gravity = 0.15; // 仿真重力影响
    float initial_velocity = 1.5; // 初速度
    float resistance_factor = 0.3; // 风阻阻力系数

    // Calculate the displacement along the normal due to initial velocity and resistance
    vec3 normal_displacement = (initial_velocity * time - 0.5 * resistance_factor * time * time) * normal; // Corrected 0.05 to 0.5 to be consistent with gravity displacement calculation

    // 计算因重力作用下落的位移，假设初速度为0，则位移为1/2 * g * t^2
    float gravity_displacement = 0.5 * discount_gravity * gravity * time * time;

    vec3 direction = normal_displacement;
    direction.y -= gravity_displacement;

    return direction;
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    float wait_time = 2.0;
    if (is_collide){
        return position + vec4(physical_simulation(time - collision_time, normal), 0.0);
    }
    return position;
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    float thickness = 0.01;
    normal = GetNormal();
    vec4 offset = vec4(thickness * normal, 0.0f);


    gl_Position = explode(gl_in[0].gl_Position, normal) - offset;
    fragPos = vec3(gl_Position);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal) - offset;
    fragPos += vec3(gl_Position);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal) - offset;
    fragPos += vec3(gl_Position);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
    fragPos /= 3;
    normal = - normal;
}
