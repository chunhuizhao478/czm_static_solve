[Mesh]
    [./msh]
      type = GeneratedMeshGenerator
      dim = 2
      nx = 300
      ny = 300
      xmin = -15000
      xmax = 15000
      ymin = -15000
      ymax = 15000
    []
    [./new_block]
        type = ParsedSubdomainMeshGenerator
        input = msh
        combinatorial_geometry = 'y<0'
        block_id = 1
    []
    [./split]
        type = BreakMeshByBlockGenerator
        input = new_block
        split_interface = true
    []
    [interface]
        type = SideSetsBetweenSubdomainsGenerator
        input = split
        primary_block = 0
        paired_block = 1
        new_boundary = 'interface'
    []
    [secondary_interface]
        type = SideSetsBetweenSubdomainsGenerator
        input = interface
        primary_block = 1
        paired_block = 0
        new_boundary = 'secondary_interface'
    []
    [sideset1]
        type = SideSetsFromNodeSetsGenerator
        input = secondary_interface
    []
    [extranodeset2]
        type = ExtraNodesetGenerator
        coord = '-15000   -15000  0; 15000    15000  0;'
        new_boundary = 2ptrs
        input = sideset1
    []
    [extranodeset4]
        type = ExtraNodesetGenerator
        coord = '-15000  -15000  0;
                 -15000   15000  0;
                  15000  -15000  0;
                  15000   15000  0;'
        new_boundary = 4ptrs
        input = extranodeset2
    []
[]

[GlobalParams]
    displacements = 'disp_x disp_y'
    #static friction
    mu_s = 0.677
    #dynamic friction
    mu_d = 0.525
    #characteristic length
    Dc = 0.4
[]

[AuxVariables]
    [./resid_x]
        order = FIRST
        family = LAGRANGE
    []
    [./resid_y]
        order = FIRST
        family = LAGRANGE
    []
[]

[Modules/TensorMechanics/CohesiveZoneMaster]
    [./czm_ik]
        boundary = 'Block0_Block1'
        strain = SMALL
        generate_output='traction_x traction_y jump_x jump_y normal_traction tangent_traction normal_jump tangent_jump'
    [../]
[]

[Modules]
    [./TensorMechanics]
        [./Master]
        [./all]
            strain = SMALL
            add_variables = true
            planar_formulation = PLANE_STRAIN
            generate_output = 'stress_xx stress_yy stress_xy'
            save_in = 'resid_x resid_y'
        [../]
        [../]
    [../]
[]

[Materials]
    [elasticity]
        type = ComputeIsotropicElasticityTensor
        lambda = 32.04e9
        shear_modulus = 32.04e9
        use_displaced_mesh = false
    []
    [stress]
        type = ComputeLinearElasticStress
    []
    [./czm_mat]
        type = SlipWeakeningMaterial
        boundary = 'Block0_Block1'
    [../]
[]

[BCs]
    #outer boundaries
    [./fix_r_x]
        type = NeumannBC
        variable = disp_x
        boundary = right
        value = -120e6
    []
    [./fix_r_y]
        type = NeumannBC
        variable = disp_y
        boundary = right
        value = 70e6
    []
    [./fix_l_x]
        type = NeumannBC
        variable = disp_x
        boundary = left
        value = 120e6
    []  
    [./fix_l_y]
        type = NeumannBC
        variable = disp_y
        boundary = left
        value = -70e6
    []       
    [./fix_t_x]
        type = NeumannBC
        variable = disp_x
        boundary = top
        value = 70e6
    []
    [./fix_t_y]
        type = NeumannBC
        variable = disp_y
        boundary = top
        value = -120e6
    []    
    [./fix_b_x]
        type = NeumannBC
        variable = disp_x
        boundary = bottom
        value = -70e6
    []
    [./fix_b_y]
        type = NeumannBC
        variable = disp_y
        boundary = bottom
        value = 120e6
    []
    [./fix_ptrs_x]
        type = DirichletBC
        variable = disp_x
        boundary = 4ptrs
        value = 0
    []
    [./fix_ptrs_y]
        type = DirichletBC
        variable = disp_y
        boundary = 4ptrs
        value = 0
    []
[]

[Executioner]
    type = Steady
    solve_type = 'NEWTON'
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = ' hypre    boomeramg'
    nl_rel_tol = 1e-11
    nl_abs_tol = 1e-50
    #num_steps = 1
[]

[Outputs]
    exodus = true
[]