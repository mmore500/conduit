find_package(Catch2 REQUIRED)

file(GLOB_RECURSE ALL_TESTS
    uitsl/*.cpp
    uit/*.cpp
    netuit/*.cpp
)

list(REMOVE_ITEM ALL_TESTS
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t::IdrOwDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Put+outlet=Window_t::IpOwDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeIrsend+outlet=BlockIrecv_t::IdirObiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Rsend+outlet=BlockIrecv_t::IrObiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_t::IrOri.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Send+outlet=RingIrecv_t::IsOriDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIrsend+outlet=RingIrecv_t::IdirOriDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIrsend+outlet=Iprobe_c::IdirOiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Rsend+outlet=Iprobe_c::IrOiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIrsend+outlet=Iprobe_s::IrirOiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIrend+outlet=BlockIrecv_s::IrirObiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIrsend+outlet=BlockIrecv_s::IrirObiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stetgpping+type=cereal/inlet=RingIrsend+outlet=Iprobe_c::IrirOiDuct.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uitsl/mpi/MpiMultithreadGuard.cpp"
    "${CMAKE_SOURCE_DIR}/tests/uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIrsend+outlet=Iprobe_c::IrirOiDuct.cpp"
)

set(CATCH_BUILD_TESTING ON)
set(BUILD_TESTING ON)

add_executable(tests
    main.cpp
    ${ALL_TESTS}
)


add_custom_command(TARGET tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data.."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/tests/uitsl/utility/assets ${CMAKE_BINARY_DIR}/tests/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_HOME_DIRECTORY}/tests/uitsl/debug/assets ${CMAKE_BINARY_DIR}/tests/assets
    )

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND /bin/python3
    )


target_link_libraries(tests Catch2::Catch2 conduit)

include(CatchInt.cmake)
include(ParseAndAddCatchTests)


set(OptionalCatchTestLauncher ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2)
#catch_discover_tests(tests)
ParseAndAddCatchTests(tests)
unset(OptionalCatchTestLauncher)
