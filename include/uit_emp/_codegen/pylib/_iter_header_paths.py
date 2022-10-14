import typing

def iter_header_paths() -> typing.Iterator[str]:
  # header_paths = glob(
  #     "../../third-party/Empirical/include/emp/**/*.hpp",
  #     recursive=True,
  # )

  yield from (
      "data/DataNode.hpp",
      "data/DataFile.hpp",
      "datastructs/QueueCache.hpp",
      "datastructs/hash_utils.hpp",
      "datastructs/tuple_utils.hpp",
      "io/ContiguousStream.hpp",
      "io/MemoryIStream.hpp",
      "math/math.hpp",
      "math/Random.hpp",
      "math/random_utils.hpp",
      "meta/meta.hpp",
      "meta/TypePack.hpp",
      "polyfill/span.hpp",
      "tools/string_utils.hpp",
      "tools/keyname_utils.hpp",
  )
