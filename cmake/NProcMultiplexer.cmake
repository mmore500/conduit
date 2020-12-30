include("../cmake/CatchInt.cmake")

function(nproc_multiplex_tests TARGET)
    foreach(nproc RANGE 1 8)
        if (MPI_EXECUTABLE_SUFFIX STREQUAL ".openmpi")
            set(QuietFlag "--quiet" "--mca" "btl_base_warn_component_unused" "0")
        else()
            set(QuietFlag "")
        endif()
        set(OptionalCatchTestLauncher ${MPIEXEC} ${QuietFlag} ${MPIEXEC_NUMPROC_FLAG} ${nproc})
        catch_discover_tests(${TARGET}
            TEST_SPEC "[nproc:${nproc}]"
            TEST_SUFFIX "-nproc${nproc}"
        )

        catch_discover_tests(${TARGET}
            TEST_SPEC "~[nproc:1]~[nproc:2]~[nproc:3]~[nproc:4]~[nproc:5]~[nproc:6]~[nproc:7]~[nproc:8]"
            TEST_SUFFIX "-nproc${nproc}"
        )

        unset(OptionalCatchTestLauncher)
    endforeach(nproc)

endfunction()
