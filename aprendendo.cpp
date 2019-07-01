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


            if (direcao == "up" and MAPA[i-1][j] != '1' and !inicial)
            indo = "up";
            if (indo == "up" and MAPA[i-1][j] != '1' and !inicial){
                i--;
                posy = i*q;
            }
            

            if (direcao == "down" and MAPA[i+1][j] != '1' and !inicial)
            indo = "down";
            if (indo == "down" and MAPA[i+1][j] != '1' and !inicial){}
                i++;
                posy = i*q;
            }
            
            if (direcao == "left" and MAPA[i][j-1] != '1' and !inicial)
            indo = "left";
            if (indo == "left" and MAPA[i][j-1] != '1' and !inicial){
                j--;
                posx = j*q;
            }

            if (direcao == "right" and MAPA[i][j+1] != '1' and !inicial)
            indo = "right";
            if (indo == "right" and MAPA[i][j+1] != '1' and !inicial){
                j++;
                posx = j*q;
            }