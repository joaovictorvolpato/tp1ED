#include <iostream>
#include <string>
#include <fstream>
#include "queue.h"
#include "stack.h"

struct Image {
    int height;
    int width;
    int **data;
    std::string name;
};

struct Pixel{
    int x;
    int y;
};

structures::LinkedStack<std::string> stack{};
structures::LinkedQueue<std::string> queue{};
structures::LinkedQueue<struct Pixel*> pixels_to_label{};
structures::LinkedQueue<struct Image*> images{};

bool xml_verification (char *arquivo) {
    std::ifstream file(arquivo);
    int j;


    if (file.is_open()) {
        std::string topo, linha, tag;

        while (getline(file, linha)) {
            for (int i = 0; i < linha.size(); i++) {
                if (linha[i] == '<') {
                    j = i;
                    while (linha[j] != '>') {
                        j++;
                    }
                    tag = linha.substr(i, j-i+1);

                    if(tag[1] == '/') {
                        if (stack.empty()) {
                            return false;
                        }

                        topo = stack.pop();
                        if(topo.substr(1,topo.size() - 2) != tag.substr(2, tag.size() - 3)) {
                            return false;
                        }
                    } else {
                        stack.push(tag);
                    }
                }
            }
        }
        if (!stack.empty()) {
            return false;
        }
        return true;
    }
    return false;
}

void coleta_tags (char *arquivo) {
    std::cout << "" << std::endl;
    std::ifstream file(arquivo);

    if (file.is_open()) {
        std::string data, linha, tag;
        
        while (getline(file, linha)) {
            int i = 0, j = 0;

            while (i < linha.size()) {
                j = i;
                if (linha[i] == '<') {
                    while (linha[j] != '>') {
                        j++;
                    }
                    tag = linha.substr(i, j-i+1);
                    i = j + 1;
                    queue.enqueue(tag);
                } else {
                    while (linha[j] != '<' && linha.size()) {
                        j++;
                    }
                    data = linha.substr(i, j-i);
                    i = j;

                    if (data[0] != '\0') {
                        queue.enqueue(data);
                    }
                }
            }
        }
    }
}

void colect_atributes() {
    std::string atribute;
    struct Image *image;

    while (!queue.empty()) {
        image = new Image();

        while (true) {

            if (atribute == "<name>") {
                image -> name = queue.dequeue();
            }
            else if (atribute == "<width>") {
                image -> width = stoi(queue.dequeue());
            }
            else if (atribute == "<height>") {
                image -> height = stoi(queue.dequeue());
            }
            else if (atribute == "<data>") {
                int **matriz = new int*[image -> height];
                for (int i = 0; i < image -> height; i++) {
                    matriz[i] = new int[image -> width];
                }
                for (int i = 0; i < image -> height; i++) {
                    atribute = queue.dequeue();
                    for (int j = 0; j < image -> width; j++) {
                        if (atribute[j]) {
                            matriz[i][j] = (int)atribute[j] - 48;
                        }
                    }
                }
                image -> data = matriz;
            }
            if (atribute == "</img>" || queue.empty()) {
                break;
            }
            atribute = queue.dequeue();
        }
        atribute = queue.dequeue();

        images.enqueue(image);
    }
}

/*void flood_fill(struct Image *image, int x, int y) {
    if (image->data[x][y] == 1) {
        image->data[x][y] = 0;
    	if (x > 0) {
            flood_fill(image, x - 1, y);
        }
    	if (y > 0) {
            flood_fill(image, x, y - 1);
        }
    	if (x < image -> height - 1) {
            flood_fill(image, x + 1, y);
        }
    	if (y < image -> width - 1) {
            flood_fill(image, x, y + 1);
        }
    }
}

int coleta_nodes(struct Image *image) {
	int con = 0;
	for (int i = 0; i < image -> height; i++) {
		for (int j = 0; j < image -> width; j++) {
			if ( image -> data[i][j] == 1) {
				con += 1;
				flood_fill(image , i, j);
			}
		}
	}
	return con;
}*/

int labeling(struct Image *image){
    struct Image *Rmatrix;
    struct Pixel *pixel;
    struct Pixel *pixelup;
    struct Pixel *pixelleft;
    struct Pixel *pixelright;
    struct Pixel *pixeldown;
    //creating R matrix
    for (int i = 0; i < image -> height; i++) {
		for (int j = 0; j < image -> width; j++) {
			    Rmatrix -> data[i][j] == 0;
		}
	}
    int label = 1;
    //iterating E matrix
    for (int i = 0; i < image -> height; i++) {
		for (int j = 0; j < image -> width; j++) {
			if (image -> data[i][j] == 1 && Rmatrix -> data[i][j] == 0){
                pixel->x = i;
                pixel->y = j;
                pixels_to_label.enqueue(pixel);
                Rmatrix -> data[i][j] == label;
            }
            while(!pixels_to_label.empty()){
                pixel = pixels_to_label.dequeue();

                //seta os pixels vizinhos
                pixelup->x = pixel->x;
                pixelup->y = pixel->y+1;
                pixeldown->x = pixel->x;
                pixeldown->y = pixel->y-1;
                pixelright->x = pixel->x+1;
                pixelright->y = pixel->y+1;
                pixelleft->x = pixel->x-1;
                pixelleft->y = pixel->y;

                //teste se esta dentro da imagem e insere na fila
                if(0 < pixel->x+1 <= image-> height)
                    pixels_to_label.enqueue(pixelright);
                if(0 < pixel->x-1 <= image-> height)
                    pixels_to_label.enqueue(pixelleft);
                if(0 < pixel->y+1 <= image-> width)
                    pixels_to_label.enqueue(pixelup);
                if(0 < pixel->y-1 <= image-> width)
                    pixels_to_label.enqueue(pixeldown);

                //testa se o pixel na matrix original =1 e se ja foi rotulado na matrix auxiliar e atribui o rotulo
                if(image -> data[pixel->x][pixel->y+1] == 1 && Rmatrix -> data[pixel->x][pixel->y+1] == 0){
                    Rmatrix -> data[pixel->x][pixel->y+1] == label;
                }
                if(image -> data[pixel->x][pixel->y-1] == 1 && Rmatrix -> data[pixel->x][pixel->y-1] == 0){
                    Rmatrix -> data[pixel->x][pixel->y-1] == label;
                }
                if(image -> data[pixel->x+1][pixel->y] == 1 && Rmatrix -> data[pixel->x+1][pixel->y] == 0){
                    Rmatrix -> data[pixel->x+1][pixel->y] == label;
                }
                if(image -> data[pixel->x-1][pixel->y] == 1 && Rmatrix -> data[pixel->x-1][pixel->y] == 0){
                    Rmatrix -> data[pixel->x-1][pixel->y] == label;
                }
            }
        label++;          
		}
	}
    return label;
}

void resultado() {
    int i;
    struct Image *binary_image;

    while (!images.empty()) {
        binary_image = images.dequeue();
        std::cout << binary_image -> name << ' ';
        std::cout << labeling(binary_image) << std::endl;
    }
}

void run(char *arquivo) {
    if (xml_verification(arquivo)) {
        coleta_tags(arquivo);
        colect_atributes();
        resultado();
    } else {
        std::cout << "error" << std::endl;
    }
}