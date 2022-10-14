import typing

def iter_header_paths() -> typing.Iterator[str]:
  # header_paths = glob(
  #     "../../third-party/Empirical/include/emp/**/*.hpp",
  #     recursive=True,
  # )

  yield from (
      "base/macros.hpp",
      "config/config.hpp",
      "data/DataNode.hpp",
      "data/DataFile.hpp",
      "datastructs/Cache.hpp",
      "datastructs/hash_utils.hpp",
      "datastructs/tuple_utils.hpp",
      "datastructs/QueueCache.hpp",
      "io/ContiguousStream.hpp",
      "io/MemoryIStream.hpp",
      "matching/matchbin_metrics.hpp",
      "matching/MatchDepository.hpp",
      "matching/regulators/PlusCountdownRegulator.hpp",
      "matching/selectors_static/RankedSelector.hpp",
      "math/Distribution.hpp",
      "math/math.hpp",
      "math/Random.hpp",
      "math/random_utils.hpp",
      "meta/meta.hpp",
      "meta/TypePack.hpp",
      "polyfill/span.hpp",
      "tools/hash_namify.hpp",
      "tools/string_utils.hpp",
      "tools/keyname_utils.hpp",
  )
