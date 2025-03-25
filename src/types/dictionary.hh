#ifndef DICTIONARY_HH
#define DICTIONARY_HH

#include <QHash>
#include <QString>
#include <QVector>

template <typename T> class Dictionary : public QHash<QString, T> {
public:
    Dictionary() {};
};

#endif