#include <network/serialization/mlcmst_serialization.hpp>


namespace MLCMST::network::serialization {

MLCMSTSerializer::~MLCMSTSerializer() = default;

void MLCMSTSerializer::serialize(const MLCMST &mlcmst, std::ostream &stream)
{
    const int N = mlcmst.vertexCount();
    stream << N << "\n";
    stream << mlcmst.root() << "\n";

    for (int i=0; i<N; i++) {
        stream << mlcmst.parent(i) << " ";
    }
    stream << "\n";
    for (int i=0; i<N; i++) {
        stream << mlcmst.edgeLevel(i) << " ";
    }
    stream << "\n";
}

MLCMSTDeserializer::~MLCMSTDeserializer() = default;

MLCMST MLCMSTDeserializer::deserialize(std::istream &stream)
{
    int N, root;
    stream >> N >> root;
    MLCMST mlcmst(N, root);

    for (int i=0; i<N; i++) {
        stream >> mlcmst.parent(i);
    }
    for (int i=0; i<N; i++) {
        stream >> mlcmst.edgeLevel(i);
    }

    return mlcmst;
}

}
