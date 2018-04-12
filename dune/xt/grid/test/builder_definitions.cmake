# ~~~
# This file is part of the dune-xt-grid project:
#   https://github.com/dune-community/dune-xt-grid
# Copyright 2009-2018 dune-xt-grid developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
# Authors:
#   Rene Milk (2017 - 2018)
# ~~~

set(DXT_BIN_COUNT "2" CACHE STRING "number of bins for test targets")
add_custom_target(test_binaries_builder_0
                  DEPENDS headercheck__dune_xt_grid_boundaryinfo_allneumann.hh
                          headercheck__dune_xt_grid_boundaryinfo_allreflecting.hh
                          headercheck__dune_xt_grid_boundaryinfo_interfaces.hh
                          headercheck__dune_xt_grid_boundaryinfo_normalbased.hh
                          headercheck__dune_xt_grid_capabilities.hh
                          headercheck__dune_xt_grid_exceptions.hh
                          headercheck__dune_xt_grid_gridprovider.hh
                          headercheck__dune_xt_grid_gridprovider_cube.hh
                          headercheck__dune_xt_grid_gridprovider_factory.hh
                          headercheck__dune_xt_grid_gridprovider_gmsh.hh
                          headercheck__dune_xt_grid_gridprovider_provider.hh
                          headercheck__dune_xt_grid_grids.hh
                          headercheck__dune_xt_grid_information.hh
                          headercheck__dune_xt_grid_intersection.hh
                          headercheck__dune_xt_grid_layers.hh
                          headercheck__dune_xt_grid_output_entity_visualization.hh
                          headercheck__dune_xt_grid_output_pgf.hh
                          headercheck__dune_xt_grid_parallel_partitioning_ranged-internal.hh
                          headercheck__dune_xt_grid_test_provider.hh
                          headercheck__dune_xt_grid_view_from-part.hh
                          headercheck__dune_xt_grid_view_periodic.hh
                          headercheck__dune_xt_grid_walker.lib.hh
                          headercheck__dune_xt_grid_walker_apply-on.hh
                          headercheck__dune_xt_grid_walker_functors.hh
                          headercheck__dune_xt_grid_walker_wrapper.hh
                          headercheck__python_dune_xt_grid_boundaryinfo.bindings.hh
                          headercheck__python_dune_xt_grid_gridprovider.hh
                          headercheck__python_dune_xt_grid_grids.bindings.hh
                          test_boundaryinfo_factory_alberta_2d
                          test_boundaryinfo_factory_alberta_3d
                          test_boundaryinfo_factory_alu_2d_0000
                          test_boundaryinfo_factory_alu_2d_0001
                          test_boundaryinfo_factory_alu_3d_0002
                          test_boundaryinfo_factory_yasp_3d
                          test_cube_gridprovider_alberta_2d
                          test_cube_gridprovider_alberta_3d
                          test_cube_gridprovider_alu_2d_0000
                          test_cube_gridprovider_alu_3d_0000
                          test_cube_gridprovider_alu_3d_0001
                          test_cube_gridprovider_yasp_3d
                          test_dd_glued_3d
                          test_dd_subdomains_cube_1d_oned_1d
                          test_dd_subdomains_cube_1d_yasp_1d
                          test_dd_subdomains_cube_2d_alu_2d_0000
                          test_dd_subdomains_cube_2d_alu_2d_0002
                          test_dd_subdomains_cube_2d_yasp_2d
                          test_dd_subdomains_cube_3d_alu_3d_0001
                          test_dd_subdomains_cube_3d_alu_3d_0002
                          test_dgf_gridprovider_alberta_1d
                          test_dgf_gridprovider_alberta_2d
                          test_dgf_gridprovider_alberta_3d
                          test_dgf_gridprovider_alu_2d_0000
                          test_dgf_gridprovider_alu_2d_0001
                          test_dgf_gridprovider_alu_3d_0000
                          test_dgf_gridprovider_alu_3d_0001
                          test_dgf_gridprovider_yasp_1d
                          test_dgf_gridprovider_yasp_3d
                          test_empty
                          test_gmsh_gridprovider_alu_2d_0002
                          test_information_3d
                          test_periodic_gridview_alberta_1d
                          test_periodic_gridview_alberta_2d
                          test_periodic_gridview_alu_2d_0000
                          test_periodic_gridview_alu_3d_0002
                          test_periodic_gridview_yasp_1d
                          test_search_alberta_2d
                          test_search_alberta_3d
                          test_search_alu_2d_0001
                          test_search_alu_3d_0001
                          test_search_alu_3d_0002
                          test_search_yasp_1d)
set_tests_properties(test_boundaryinfo_factory_alberta_2d_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_boundaryinfo_factory_alberta_3d_0005 PROPERTIES LABELS "builder_0")
set_tests_properties(test_boundaryinfo_factory_alu_2d_0000_0006 PROPERTIES LABELS "builder_0")
set_tests_properties(test_boundaryinfo_factory_alu_2d_0001_0007 PROPERTIES LABELS "builder_0")
set_tests_properties(test_boundaryinfo_factory_alu_3d_0002_0011 PROPERTIES LABELS "builder_0")
set_tests_properties(test_boundaryinfo_factory_yasp_3d_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_alberta_2d_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_alberta_3d_0005 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_alu_2d_0000_0006 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_alu_3d_0000_0009 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_alu_3d_0001_0010 PROPERTIES LABELS "builder_0")
set_tests_properties(test_cube_gridprovider_yasp_3d_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_glued_3d PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_1d_oned_1d_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_1d_yasp_1d_0001 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_2d_alu_2d_0000_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_2d_alu_2d_0002_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_2d_yasp_2d_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_3d_alu_3d_0001_0003 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dd_subdomains_cube_3d_alu_3d_0002_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alberta_1d_0003 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alberta_2d_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alberta_3d_0005 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alu_2d_0000_0006 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alu_2d_0001_0007 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alu_3d_0000_0009 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_alu_3d_0001_0010 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_yasp_1d_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_dgf_gridprovider_yasp_3d_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_empty PROPERTIES LABELS "builder_0")
set_tests_properties(test_gmsh_gridprovider_alu_2d_0002_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_information_3d_information_3d PROPERTIES LABELS "builder_0")
set_tests_properties(test_periodic_gridview_alberta_1d_nonperiodic_rectangle_simplex_0000
                     test_periodic_gridview_alberta_1d_nonperiodic_unit_cube_simplex_0000
                     test_periodic_gridview_alberta_1d_partially_periodic_rectangle_simplex_0000
                     test_periodic_gridview_alberta_1d_partially_periodic_unit_cube_simplex_0000
                     test_periodic_gridview_alberta_1d_periodic_rectangle_simplex_0000
                     test_periodic_gridview_alberta_1d_periodic_unit_cube_simplex_0000
                     PROPERTIES LABELS "builder_0")
set_tests_properties(test_periodic_gridview_alberta_2d_nonperiodic_rectangle_simplex_0001
                     test_periodic_gridview_alberta_2d_nonperiodic_unit_cube_simplex_0001
                     test_periodic_gridview_alberta_2d_partially_periodic_rectangle_simplex_0001
                     test_periodic_gridview_alberta_2d_partially_periodic_unit_cube_simplex_0001
                     test_periodic_gridview_alberta_2d_periodic_rectangle_simplex_0001
                     test_periodic_gridview_alberta_2d_periodic_unit_cube_simplex_0001
                     PROPERTIES LABELS "builder_0")
set_tests_properties(test_periodic_gridview_alu_2d_0000_nonperiodic_rectangle_cube_0003
                     test_periodic_gridview_alu_2d_0000_nonperiodic_unit_cube_cube_0003
                     test_periodic_gridview_alu_2d_0000_partially_periodic_rectangle_cube_0003
                     test_periodic_gridview_alu_2d_0000_partially_periodic_unit_cube_cube_0003
                     test_periodic_gridview_alu_2d_0000_periodic_rectangle_cube_0003
                     test_periodic_gridview_alu_2d_0000_periodic_unit_cube_cube_0003
                     PROPERTIES LABELS "builder_0")
set_tests_properties(test_periodic_gridview_alu_3d_0002_nonperiodic_rectangle_simplex_0006
                     test_periodic_gridview_alu_3d_0002_nonperiodic_unit_cube_simplex_0006
                     test_periodic_gridview_alu_3d_0002_partially_periodic_rectangle_simplex_0006
                     test_periodic_gridview_alu_3d_0002_partially_periodic_unit_cube_simplex_0006
                     test_periodic_gridview_alu_3d_0002_periodic_rectangle_simplex_0006
                     test_periodic_gridview_alu_3d_0002_periodic_unit_cube_simplex_0006
                     PROPERTIES LABELS "builder_0")
set_tests_properties(test_periodic_gridview_yasp_1d_nonperiodic_rectangle_cube_0000
                     test_periodic_gridview_yasp_1d_nonperiodic_unit_cube_cube_0000
                     test_periodic_gridview_yasp_1d_partially_periodic_rectangle_cube_0000
                     test_periodic_gridview_yasp_1d_partially_periodic_unit_cube_cube_0000
                     test_periodic_gridview_yasp_1d_periodic_rectangle_cube_0000
                     test_periodic_gridview_yasp_1d_periodic_unit_cube_cube_0000
                     PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_alberta_2d_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_alberta_3d_0005 PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_alu_2d_0001_0007 PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_alu_3d_0001_0010 PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_alu_3d_0002_0011 PROPERTIES LABELS "builder_0")
set_tests_properties(test_search_yasp_1d_0000 PROPERTIES LABELS "builder_0")
add_custom_target(test_binaries_builder_1
                  DEPENDS headercheck__dune_xt_grid_boundaryinfo.hh
                          headercheck__dune_xt_grid_boundaryinfo_alldirichlet.hh
                          headercheck__dune_xt_grid_boundaryinfo_boundarysegment.hh
                          headercheck__dune_xt_grid_boundaryinfo_factory.hh
                          headercheck__dune_xt_grid_boundaryinfo_types.hh
                          headercheck__dune_xt_grid_dd_glued.hh
                          headercheck__dune_xt_grid_dd_subdomains_factory.hh
                          headercheck__dune_xt_grid_dd_subdomains_grid.hh
                          headercheck__dune_xt_grid_entity.hh
                          headercheck__dune_xt_grid_gridprovider_cube.lib.hh
                          headercheck__dune_xt_grid_gridprovider_dgf.hh
                          headercheck__dune_xt_grid_gridprovider_eoc.hh
                          headercheck__dune_xt_grid_gridprovider_provider.lib.hh
                          headercheck__dune_xt_grid_gridprovider_starcd.hh
                          headercheck__dune_xt_grid_parallel_partitioning_ranged.hh
                          headercheck__dune_xt_grid_periodic_gridview.hh
                          headercheck__dune_xt_grid_rangegenerators.hh
                          headercheck__dune_xt_grid_search.hh
                          headercheck__dune_xt_grid_structuredgridfactory.hh
                          headercheck__dune_xt_grid_test_dd_glued.hh
                          headercheck__dune_xt_grid_type_traits.hh
                          headercheck__dune_xt_grid_view_subdomain_entity-iterator.hh
                          headercheck__dune_xt_grid_view_subdomain_indexset.hh
                          headercheck__dune_xt_grid_view_subdomain_intersection-iterator.hh
                          headercheck__dune_xt_grid_view_subdomain_intersection-wrapper.hh
                          headercheck__dune_xt_grid_view_subdomain_view.hh
                          headercheck__dune_xt_grid_walk_functors.hh
                          headercheck__dune_xt_grid_walker.hh
                          headercheck__python_dune_xt_grid_layers.bindings.hh
                          headercheck__python_dune_xt_grid_walker.bindings.hh
                          test_boundaryinfo_factory_alberta_1d
                          test_boundaryinfo_factory_alu_2d_0002
                          test_boundaryinfo_factory_alu_3d_0000
                          test_boundaryinfo_factory_alu_3d_0001
                          test_boundaryinfo_factory_yasp_1d
                          test_boundaryinfo_factory_yasp_2d
                          test_cube_gridprovider_alberta_1d
                          test_cube_gridprovider_alu_2d_0001
                          test_cube_gridprovider_alu_2d_0002
                          test_cube_gridprovider_alu_3d_0002
                          test_cube_gridprovider_yasp_1d
                          test_cube_gridprovider_yasp_2d
                          test_dd_glued_2d
                          test_dd_subdomains_cube_1d_alberta_1d
                          test_dd_subdomains_cube_2d_alberta_2d
                          test_dd_subdomains_cube_2d_alu_2d_0001
                          test_dd_subdomains_cube_3d_alberta_3d
                          test_dd_subdomains_cube_3d_alu_3d_0000
                          test_dd_subdomains_cube_3d_yasp_3d
                          test_dgf_gridprovider_alu_2d_0002
                          test_dgf_gridprovider_alu_3d_0002
                          test_dgf_gridprovider_yasp_2d
                          test_gmsh_gridprovider_alu_2d_0000
                          test_gmsh_gridprovider_alu_2d_0001
                          test_gmsh_gridprovider_alu_3d_0000
                          test_gmsh_gridprovider_alu_3d_0001
                          test_gmsh_gridprovider_alu_3d_0002
                          test_information_1d
                          test_information_2d
                          test_output_pgf
                          test_periodic_gridview_alberta_3d
                          test_periodic_gridview_alu_2d_0001
                          test_periodic_gridview_alu_2d_0002
                          test_periodic_gridview_alu_3d_0000
                          test_periodic_gridview_alu_3d_0001
                          test_periodic_gridview_yasp_2d
                          test_periodic_gridview_yasp_3d
                          test_search_alberta_1d
                          test_search_alu_2d_0000
                          test_search_alu_2d_0002
                          test_search_alu_3d_0000
                          test_search_yasp_2d
                          test_search_yasp_3d)
set_tests_properties(test_boundaryinfo_factory_alberta_1d_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_boundaryinfo_factory_alu_2d_0002_0008 PROPERTIES LABELS "builder_1")
set_tests_properties(test_boundaryinfo_factory_alu_3d_0000_0009 PROPERTIES LABELS "builder_1")
set_tests_properties(test_boundaryinfo_factory_alu_3d_0001_0010 PROPERTIES LABELS "builder_1")
set_tests_properties(test_boundaryinfo_factory_yasp_1d_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_boundaryinfo_factory_yasp_2d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_alberta_1d_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_alu_2d_0001_0007 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_alu_2d_0002_0008 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_alu_3d_0002_0011 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_yasp_1d_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_cube_gridprovider_yasp_2d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_glued_2d PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_1d_alberta_1d_0002 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_2d_alberta_2d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_2d_alu_2d_0001_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_3d_alberta_3d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_3d_alu_3d_0000_0002 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dd_subdomains_cube_3d_yasp_3d_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dgf_gridprovider_alu_2d_0002_0008 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dgf_gridprovider_alu_3d_0002_0011 PROPERTIES LABELS "builder_1")
set_tests_properties(test_dgf_gridprovider_yasp_2d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_gmsh_gridprovider_alu_2d_0000_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_gmsh_gridprovider_alu_2d_0001_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_gmsh_gridprovider_alu_3d_0000_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_gmsh_gridprovider_alu_3d_0001_0004 PROPERTIES LABELS "builder_1")
set_tests_properties(test_gmsh_gridprovider_alu_3d_0002_0005 PROPERTIES LABELS "builder_1")
set_tests_properties(test_information_1d_information_1d PROPERTIES LABELS "builder_1")
set_tests_properties(test_information_2d_information_2d PROPERTIES LABELS "builder_1")
set_tests_properties(test_output_pgf PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_alberta_3d_nonperiodic_rectangle_simplex_0002
                     test_periodic_gridview_alberta_3d_nonperiodic_unit_cube_simplex_0002
                     test_periodic_gridview_alberta_3d_partially_periodic_rectangle_simplex_0002
                     test_periodic_gridview_alberta_3d_partially_periodic_unit_cube_simplex_0002
                     test_periodic_gridview_alberta_3d_periodic_rectangle_simplex_0002
                     test_periodic_gridview_alberta_3d_periodic_unit_cube_simplex_0002
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_alu_2d_0001_nonperiodic_rectangle_simplex_0003
                     test_periodic_gridview_alu_2d_0001_nonperiodic_unit_cube_simplex_0003
                     test_periodic_gridview_alu_2d_0001_partially_periodic_rectangle_simplex_0003
                     test_periodic_gridview_alu_2d_0001_partially_periodic_unit_cube_simplex_0003
                     test_periodic_gridview_alu_2d_0001_periodic_rectangle_simplex_0003
                     test_periodic_gridview_alu_2d_0001_periodic_unit_cube_simplex_0003
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_alu_2d_0002_nonperiodic_rectangle_simplex_0004
                     test_periodic_gridview_alu_2d_0002_nonperiodic_unit_cube_simplex_0004
                     test_periodic_gridview_alu_2d_0002_partially_periodic_rectangle_simplex_0004
                     test_periodic_gridview_alu_2d_0002_partially_periodic_unit_cube_simplex_0004
                     test_periodic_gridview_alu_2d_0002_periodic_rectangle_simplex_0004
                     test_periodic_gridview_alu_2d_0002_periodic_unit_cube_simplex_0004
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_alu_3d_0000_nonperiodic_rectangle_cube_0004
                     test_periodic_gridview_alu_3d_0000_nonperiodic_unit_cube_cube_0004
                     test_periodic_gridview_alu_3d_0000_partially_periodic_rectangle_cube_0004
                     test_periodic_gridview_alu_3d_0000_partially_periodic_unit_cube_cube_0004
                     test_periodic_gridview_alu_3d_0000_periodic_rectangle_cube_0004
                     test_periodic_gridview_alu_3d_0000_periodic_unit_cube_cube_0004
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_alu_3d_0001_nonperiodic_rectangle_simplex_0005
                     test_periodic_gridview_alu_3d_0001_nonperiodic_unit_cube_simplex_0005
                     test_periodic_gridview_alu_3d_0001_partially_periodic_rectangle_simplex_0005
                     test_periodic_gridview_alu_3d_0001_partially_periodic_unit_cube_simplex_0005
                     test_periodic_gridview_alu_3d_0001_periodic_rectangle_simplex_0005
                     test_periodic_gridview_alu_3d_0001_periodic_unit_cube_simplex_0005
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_yasp_2d_nonperiodic_rectangle_cube_0001
                     test_periodic_gridview_yasp_2d_nonperiodic_unit_cube_cube_0001
                     test_periodic_gridview_yasp_2d_partially_periodic_rectangle_cube_0001
                     test_periodic_gridview_yasp_2d_partially_periodic_unit_cube_cube_0001
                     test_periodic_gridview_yasp_2d_periodic_rectangle_cube_0001
                     test_periodic_gridview_yasp_2d_periodic_unit_cube_cube_0001
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_periodic_gridview_yasp_3d_nonperiodic_rectangle_cube_0002
                     test_periodic_gridview_yasp_3d_nonperiodic_unit_cube_cube_0002
                     test_periodic_gridview_yasp_3d_partially_periodic_rectangle_cube_0002
                     test_periodic_gridview_yasp_3d_partially_periodic_unit_cube_cube_0002
                     test_periodic_gridview_yasp_3d_periodic_rectangle_cube_0002
                     test_periodic_gridview_yasp_3d_periodic_unit_cube_cube_0002
                     PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_alberta_1d_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_alu_2d_0000_0006 PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_alu_2d_0002_0008 PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_alu_3d_0000_0009 PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_yasp_2d_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_search_yasp_3d_0002 PROPERTIES LABELS "builder_1")
