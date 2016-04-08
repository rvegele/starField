//#version 120
//uniform sampler2D tex;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

varying vec2 texcoord;
varying vec4 vVertex;
uniform float time;

uniform float starSize;


float rand( vec2 co ){
    return fract( sin( dot( co.xy ,vec2( 12.9898,78.233 ) ) ) * 43758.5453 );
}


void main () {
    // jeigu noriu, kad butu ryskesnes, yra 'double up' funkcija pagrindiniam faile,
    // kuri piesia antra zvagzde (dvigubai mazesne ant virsaus);
    
    
    vec4 starShp = texture2D(tex0, texcoord);
    vec3 spectrumCol = texture2D( tex1, vec2( (1.0-starSize), 0.50 ) ).rgb; // for single spectral line
    //vec3 spectrumCol = texture2D( tex1, vec2( starSize, 0.25 ) ).rgb; // for multi spectral line
    vec4 glow = texture2D(tex2, texcoord);
    //float alpha	= starShp.a;
    //vec3 onColor = vec3( spectrumCol );
    
    gl_FragColor.rgb = spectrumCol;
    //gl_FragColor.rgb = vec3(1.0, 0.0, 0.0);
    //gl_FragColor.a = 1.0;
    
    //gl_FragColor.a = starShp.a;
    float alpha			= float ( glow.r );
    //alpha = alpha * 0.5;
    
    //float noise = rand( vec2( vVertex.xy + time ) );
    //vec4 darkRoomColor	= vec4( spectrumCol, 0.05 + noise * 0.2 );
    
    //gl_FragColor.rgb = /*vec3(gl_Color).rgb +*/ darkRoomColor.rgb;
    gl_FragColor.a = alpha;///*starShp.a;*/(glow.r+glow.g+glow.b);
    
    
}