if(key[KEY_UP] && MAPA[i-1][j] != '1' and inicial == false) {
                direcao = "up";
                while (movimento){
                    if  (direcao == "up"){
                        i--;
                        posy = i*q;
                    }
                    if (key[KEY_DOWN]) direcao == "down";
                    if (key[KEY_RIGHT]) direcao == "right";
                    if (key[KEY_LEFT]) direcao == "left";
                       
                }
            }