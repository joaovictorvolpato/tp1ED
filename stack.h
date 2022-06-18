#ifndef STACK_H
#define STACK_H

#include <stdexcept>

namespace structures {
    template<typename T>
    class LinkedStack {
      public:
            /* Construtor */
            LinkedStack();
            /* Destrutor */
            ~LinkedStack();
            /* Limpa */
            void clear();
            /* Empilha */
            void push(const T& data);
            /* Desempilha */
            T pop();
            /* Data do topo  */
            T& top() const;
            /* Verifica se esta vazia  */
            bool empty() const;
            /* Verifica tamnho  */
            std::size_t size() const;
      private:
            class Node {
              public:
                    explicit Node(const T& data) {
                        data_ = data;
                    }
                    Node(const T& data, Node* next) {
                        data_ = data;
                        next_ = next;
                    }
                    T& data() {
                        return data_;
                    }
                    const T& data() const {
                        return data_;
                    }
                    Node* next() {
                        return next_;
                    }
                    const Node* next() const {
                        return next_;
                    }
                    void next(Node* next) {
                        next_ = next;
                    }
              private:
                    T data_;
                    Node* next_;
            };

            Node* top_;
            std::size_t size_;
    };
}   // namespace structures

#endif

template<typename T>
structures::LinkedStack<T>::LinkedStack() {
    size_ = 0;
    top_ = nullptr;
}

template<typename T>
structures::LinkedStack<T>::~LinkedStack() {
    clear();
}

template<typename T>
void structures::LinkedStack<T>::clear() {
    Node *proximo, *atual = top_;
    while (atual) {
        proximo = atual;
        atual = atual -> next();
        delete proximo;
    }
    size_ = 0;
    top_ = nullptr;
}

template<typename T>
void structures::LinkedStack<T>::push(const T& data) {
    Node *novo = new Node(data,top_);
    top_ = novo;
    size_++;
}

template<typename T>
T structures::LinkedStack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Lista vazia!");
    }
    Node *pop = top_;
    T data = pop -> data();
    top_ = top_ -> next();
    size_--;
    delete pop;
    return data;
}

template<typename T>
T& structures::LinkedStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("Lista vazia!");
    }
    return top_ -> data();
}

template<typename T>
bool structures::LinkedStack<T>::empty() const {
    return (!top_);
}

template<typename T>
std::size_t structures::LinkedStack<T>::size() const {
    return (size_);
}


