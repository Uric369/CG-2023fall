#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    vec2 texCoords;
} vs_out;

// 传递给片段着色器的输出
out vec3 fragPos;
out vec3 normal;
out vec2 TexCoords; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 direction;
uniform vec3 new_direction;
uniform vec3 ini_pos;
uniform float speed;
uniform float time;
uniform bool is_collide;
uniform float collision_time;


void main()
{  
    vec3 newPos;
    float gravity = 9.8; // 重力加速度（单位时间内速度的增加量）
    float discount_gravity = 0.0015; // 仿真重力影响
    float e = 0.3;
    if (!is_collide) {newPos = direction * speed * time + aPos;}
    else {
        newPos =  direction * speed * collision_time + aPos - e * new_direction * speed * (time - collision_time);
    }
    newPos.y -= gravity * discount_gravity * time * time;
    gl_Position = projection * view * model * vec4(newPos, 1.0);
    fragPos = newPos;
    normal = normalize(aPos - ini_pos);
}
