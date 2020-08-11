#include <network/serialization/mlcst_serialization.hpp>


namespace MLCMST::network::serialization {

MLCSTSerializer::~MLCSTSerializer() = default;

void MLCSTSerializer::serialize(const MLCST &mlcmst, std::ostream &stream)
{
    const int N = mlcmst.vertexCount();
    stream << N << "\n";
    stream << mlcmst.root() << "\n";

    for (int i=0; i<N; i++) {
        stream << mlcmst.parent(i) << " ";
    }
    stream << "\n";
    for (int i=0; i<N; i++) {
        stream << mlcmst.facilityLevel(i) << " ";
    }
    stream << "\n";
}

MLCMSTDeserializer::~MLCMSTDeserializer() = default;

MLCST MLCMSTDeserializer::deserialize(std::istream &stream)
{
    int N, root;
    stream >> N >> root;
    MLCST mlcmst(N, root);

    for (int i=0; i<N; i++) {
        stream >> mlcmst.parent(i);
    }
    for (int i=0; i<N; i++) {
        stream >> mlcmst.facilityLevel(i);
    }

    return mlcmst;
}

}
