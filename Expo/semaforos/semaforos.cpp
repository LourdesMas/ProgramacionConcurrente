//Código básado en los ejemplos de Qt Documentation
#include <QThread>
#include <QSemaphore>
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>

const int DataSize = 100000;

const int BufferSize = 10;
int buffer[BufferSize];


QSemaphore freeBytes(BufferSize);
//comienza con valor de 0
QSemaphore usedBytes;

class Producer : public QThread
{
public:
    void run() override
    {
        //DataSize mayor que bufferSize
        for (int i = 0; i < DataSize; ++i) {
            freeBytes.acquire();
            buffer[i % BufferSize] = i % BufferSize;//"ACGT"[QRandomGenerator::global()->bounded(4)];
            fprintf(stderr, "Productor produce el %d\n", buffer[i % BufferSize]);
            usedBytes.release();
        }
    }
};

//heredeamos de la clase QThread
class Consumer : public QThread
{
    Q_OBJECT
public:
    void run() override
    {
        for (int i = 0; i < DataSize; ++i) {
            //empieza bloqueado a que el semaforo sea liberado
            usedBytes.acquire();
            fprintf(stderr, "Consumidor consume el %d\n", buffer[i % BufferSize]);
            freeBytes.release();
        }
        fprintf(stderr, "\n");
    }

signals:
    void stringConsumed(const QString &text);

protected:
    bool finish;
};


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    //waits para asegurar que los hilos han terminado antes de terminar
    producer.wait();
    consumer.wait();
    return 0;
}
//meta object compiler
#include "semaforos.moc"