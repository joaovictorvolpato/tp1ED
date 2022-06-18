#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

namespace structures {
    template<typename T>
    class LinkedQueue {
      public:
            /* Construtor */
            LinkedQueue();
            /* Destrutor */
            ~LinkedQueue();
            /* Limpa */
            void clear();
            /* Bota na fila */
            void enqueue(const T& data);
            /* Tira da fila */
            T dequeue();
            /* Primeiro da fila */
            T& front() const;
            /* Ultimo da fila */
            T& back() const;
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

            Node* head;
            Node* tail;
            std::size_t size_;
    };
}   // namespace structures

#endif

template<typename T>
structures::LinkedQueue<T>::LinkedQueue() {
    size_ = 0;
    head = nullptr;
    tail = nullptr;
}

template<typename T>
structures::LinkedQueue<T>::~LinkedQueue() {
    clear();
}

template<typename T>
void structures::LinkedQueue<T>::clear() {
    Node *proximo, *atual = head;
    while (atual) {
        proximo = atual;
        atual = atual -> next();
        delete proximo;
    }
    size_ = 0;
    head = nullptr;
    tail = nullptr;
}

template<typename T>
void structures::LinkedQueue<T>::enqueue(const T& data) {
    Node *novo = new Node(data, nullptr);
    if (empty()) {
        head = novo;
        tail = novo;
    } else {
        tail -> next(novo);
        tail = novo;
    }
    size_++;
}

template<typename T>
T structures::LinkedQueue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("Lista vazia!");
    }
    Node *data = head;
    T returnData = data -> data();
    head = head -> next();
    size_--;
    delete data;
    return returnData;
}


template<typename T>
T& structures::LinkedQueue<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Lista vazia!");
    }
    return tail -> data();
}

template<typename T>
T& structures::LinkedQueue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Lista vazia!");
    }
    return head -> data();
}

template<typename T>
bool structures::LinkedQueue<T>::empty() const {
    return (!head);
}

template<typename T>
std::size_t structures::LinkedQueue<T>::size() const {
    return (size_);
}
