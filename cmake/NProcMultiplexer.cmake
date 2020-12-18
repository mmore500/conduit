include("../cmake/CatchInt.cmake")

function(nproc_multiplex_tests TARGET)
    foreach(nproc RANGE 1 8)
        set(OptionalCatchTestLauncher ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${nproc})
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


