with import <nixpkgs> {}; {
     sdlEnv = stdenv.mkDerivation {
       name = "sdl";
       buildInputs = [ stdenv SDL2 ];
     };
   }