//Código básado en los ejemplos de Qt Documentation
#include <QtCore>

#include <stdio.h>
#include <stdlib.h>

const int DataSize = 100000;

const int BufferSize = 10;
int buffer[BufferSize];

QWaitCondition bufferNotEmpty;
QWaitCondition bufferNotFull;
QMutex mutex;
int numUsedBytes = 0;

class Producer : public QThread
{
public:
    Producer(QObject *parent = NULL) : QThread(parent)
    {
    }

    void run() override
    {
        for (int i = 0; i < DataSize; ++i) {
            mutex.lock();
            if (numUsedBytes == BufferSize)
                bufferNotFull.wait(&mutex);
            mutex.unlock();

            buffer[i % BufferSize] = i % BufferSize;//[QRandomGenerator::global()->bounded(4)];
            fprintf(stderr, "Productor produce el %d\n", buffer[i % BufferSize]);
            mutex.lock();
            ++numUsedBytes;
            bufferNotEmpty.wakeAll();
            mutex.unlock();
        }
    }
};

class Consumer : public QThread
{
    Q_OBJECT
public:
    Consumer(QObject *parent = NULL) : QThread(parent)
    {
    }

    void run() override
    {
        for (int i = 0; i < DataSize; ++i) {
            mutex.lock();
            if (numUsedBytes == 0)
                bufferNotEmpty.wait(&mutex);
            mutex.unlock();

            fprintf(stderr, "Consumidor consume el %d\n", buffer[i % BufferSize]);

            mutex.lock();
            --numUsedBytes;
            bufferNotFull.wakeAll();
            mutex.unlock();
        }
        fprintf(stderr, "\n");
    }

signals:
    void stringConsumed(const QString &text);
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return 0;
}
//meta object compiler
#include "waits.moc"