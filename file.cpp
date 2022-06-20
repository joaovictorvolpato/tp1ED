#include <iostream>
#include <string>
#include <fstream>
#include "queue.h"
#include "stack.h"

//strutura para tratar as imagens binarias
struct Image {
    int height;
    int width;
    int **data;
    std::string name;
};

//estrutura da dados nescessarias para tratar o problema
structures::LinkedStack<std::string> stack{};
structures::LinkedQueue<std::string> queue{};
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

void recursive_fill(struct Image *image, int x, int y) {
    if (image->data[x][y] == 1) {
        image->data[x][y] = 0;
    	if (x > 0) {
            recursive_fill(image, x - 1, y);
        }
    	if (y > 0) {
            recursive_fill(image, x, y - 1);
        }
    	if (x < image -> height - 1) {
            recursive_fill(image, x + 1, y);
        }
    	if (y < image -> width - 1) {
            recursive_fill(image, x, y + 1);
        }
    }
}

int get_conected_components(struct Image *image) {
	int conected_components = 1;
	for (int i = 0; i < image -> height; i++) {
		for (int j = 0; j < image -> width; j++) {
			if ( image -> data[i][j] == 1) {
				conected_components += 1;
				recursive_fill(image , i, j);
			}
		}
	}
	return conected_components-1;
}

void resultado() {
    int i;
    struct Image *binary_image;

    while (!images.empty()) {
        binary_image = images.dequeue();
        std::cout << binary_image -> name << ' ';
        std::cout << get_conected_components(binary_image) << std::endl;
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