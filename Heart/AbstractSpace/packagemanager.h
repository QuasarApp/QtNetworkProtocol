#ifndef PAKCAGEMANAGER_H
#define PAKCAGEMANAGER_H

#include <QMutex>
#include <QSharedDataPointer>
#include <baseid.h>


namespace QH {

class PackaData;

/**
 * @brief The PakcageManager class contains all processed packages. Working like a cache of packages.
 */
class PackageManager
{
public:
    PackageManager();
    ~PackageManager();

    /**
     * @brief getPkgFromArhive This method return pointer tot package from arhive.
     * @param id This is id of the requariment package.
     * @result pointer of the processed package, if package not findet then return nullptr.
     */
    const Package * getPkgFromArhive(const unsigned int &id) const;

    /**
     * @brief contains This method checks if the packge contains in this container by id.
     * @param id This is unique key of package.
     * In Default implementation it is qHash of pacakge.
     * @return True if the pakcge has been parsed
     */
    bool contains(const unsigned int& id) const;

    /**
     * @brief processed This method add a package to the archive.
     * @param pkg This is object of the package
     * @param processResult This is result of method parsePackage.
     * For more information see ParserResult enum.
     */
    void processed(const Package& pkg, char processResult);
private:

    QMultiMap<int, unsigned int> _processTime;
    QHash<unsigned int, const PackaData*> _parseResults;

    mutable QMutex _processMutex;
};

}
#endif // PAKCAGEMANAGER_H