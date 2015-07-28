#include <rose.h>
#include <stp_schema.h>
#include "geometry_utils.h"
#include "context_utils.h"
#include "unit_utils.h"

stp_advanced_brep_shape_representation * create_empty_product_with_geometry();
void create_any_cylinder(stp_advanced_brep_shape_representation * advsol, double rad, double hei);

int main(int argc, char* argv[])
{
	/* Force optimizing linkers to bring in all C++ classes */
	stplib_init();

	/* Create a RoseDesign to hold the instance data */
	const char * output_name = "cylinder";
	RoseDesign * design = new RoseDesign(output_name);

	/* Give the design some Part 21 header information */
	design->initialize_header();
	design->header_name()->originating_system("Cylinder Creation");
	design->header_description()->description()->
		add("Empty Shape Representation Sample File");

	ROSE.useDesign(design);

	stp_advanced_brep_shape_representation * cylin = create_empty_product_with_geometry();

	create_any_cylinder(cylin, 5, 10);

	design->save();
	return 0;
}

stp_advanced_brep_shape_representation * create_empty_product_with_geometry()
{
	//----------------------------------------
	// CREATE THE PRODUCT -
	// Create a product object to represent our part. 
	stp_product* pr = pnew stp_product();

	// Fill in the part number, name, and description. 
	pr->id("1234-K789");
	pr->name("Cylinder");
	pr->description("the start of something new");

	// The context information is boilerplate information that is the
	// same for all files defined by a particular AP, although it does
	// change a bit from AP to AP.  See the context_utils.cxx file for
	// the specifics.
	//
	pr->frame_of_reference()->add(make_product_context());


	// The product needs a product definition formation to describe
	// the version with an id and description
	// 
	stp_product_definition_formation* pdf =
		pnew stp_product_definition_formation;

	pdf->of_product(pr);
	pdf->id("1.0");
	pdf->description("first version of cylinder creator");


	// The PDF needs a product definition.  This describes the
	// version from a particular point of view in the life cycle, so
	// most APs only have one per PDF, but some APs use several.

	stp_product_definition* pd = pnew stp_product_definition();
	pd->formation(pdf);
	pd->id("design");
	pd->description("Great Cylinder");

	// The context information is boilerplate information as above in
	// the product.  See the context_utils.cxx file for the specifics.
	// 
	pd->frame_of_reference(make_pdef_context());

	stp_advanced_brep_shape_representation * rep = pnew stp_advanced_brep_shape_representation;

	// Give the product a shape property.  Product_definition_shape is
	// a subtype of property_definition used for shape properties.  It 
	// refers to the product through the product_definition.
	//
	stp_product_definition_shape * pds =
		pnew stp_product_definition_shape;

	pds->name("");
	pds->definition(pnew stp_characterized_definition);
	pds->definition()->_characterized_product_definition
		(pnew stp_characterized_product_definition);
	pds->definition()->_characterized_product_definition()->
		_product_definition(pd);


	// Attach the shape representation to the property.  The
	// shape_definition_representation subtype is used for shape
	// properties and the property_definition_representation supertype
	// is used for other types of properties.
	//
	stp_shape_definition_representation * sdr =
		pnew stp_shape_definition_representation;

	sdr->definition(pnew stp_represented_definition);
	sdr->definition()->_property_definition(pds);
	sdr->used_representation(rep);

	stp_representation_context * rep_ctx = make_geometry_context(
		ROSE.design(),	// location to create the context
		"ID1", 		// some name for the context
		3, 		// number of dimensions, ie 3D
		make_mm_unit(), 	// length unit for geometry
		make_degree_unit(),	// angle unit for geometry
		make_steradian_unit(),	// solid angle unit
		1e-6
		);

	rep->name("");
	rep->context_of_items(rep_ctx);

	return rep;
}

void create_any_cylinder(stp_advanced_brep_shape_representation * advsol, double diam, double hei){
	
	//Variable to create radius entity for various entities used
	double temp = diam / 2.0;

	//Final axis placements and cartesian points/vectors
	stp_cartesian_point* p1 = pnew stp_cartesian_point;
	p1->name("");
	p1->coordinates()->add(0);
	p1->coordinates()->add(0);
	p1->coordinates()->add(0);

	stp_direction * dir1 = pnew stp_direction;
	dir1->direction_ratios()->add(0);
	dir1->direction_ratios()->add(0);
	dir1->direction_ratios()->add(1);

	stp_direction * dir2 = pnew stp_direction;
	dir2->direction_ratios()->add(1);
	dir2->direction_ratios()->add(0);
	dir2->direction_ratios()->add(0);

	stp_axis2_placement_3d * a2p32 = pnew stp_axis2_placement_3d("", p1, dir1, dir2);

	stp_axis2_placement * a2p4 = pnew stp_axis2_placement;
	rose_put_nested_object(a2p4, a2p32);

	stp_cylindrical_surface * cs1 = pnew stp_cylindrical_surface;
	cs1->position(a2p32);
	cs1->radius(temp);

	stp_direction * dir5 = pnew stp_direction;
	dir5->direction_ratios()->add(1);
	dir5->direction_ratios()->add(0);

	stp_direction * dir6 = pnew stp_direction;
	dir6->direction_ratios()->add(0);
	dir6->direction_ratios()->add(1);

	
	stp_cartesian_point* p5 = pnew stp_cartesian_point;
	p5->name("");
	p5->coordinates()->add(0);
	p5->coordinates()->add(0);
	p5->coordinates()->add(hei);
	
	stp_cartesian_point* p6 = pnew stp_cartesian_point;
	p6->name("");
	p6->coordinates()->add(0);
	p6->coordinates()->add(hei);

	stp_cartesian_point* p10 = pnew stp_cartesian_point;
	p10->name("");
	p10->coordinates()->add(0);
	p10->coordinates()->add(0);

	stp_vector * vec2 = pnew stp_vector;
	vec2->orientation(dir5);
	vec2->magnitude(1);

	stp_axis2_placement_2d * a2p2d2 = pnew stp_axis2_placement_2d;
	a2p2d2->location(p10);
	a2p2d2->ref_direction(dir5);

	stp_axis2_placement * a2p3 = pnew stp_axis2_placement;
	rose_put_nested_object(a2p3, a2p2d2);

	stp_cartesian_point* p7 = pnew stp_cartesian_point;
	p7->name("");
	p7->coordinates()->add(6.28318530718);
	p7->coordinates()->add(0);

	stp_vector * vec3 = pnew stp_vector;
	vec3->orientation(dir6);
	vec3->magnitude(1);

	
	//Line and circle definitions
	stp_line * l2 = pnew stp_line;
	l2->pnt(p6);
	l2->dir(vec2);

	stp_geometric_representation_context * rc1 = pnew stp_geometric_representation_context;
	rc1->context_identifier("REPRESENTATION_CONTEXT('2D SPACE','')");
	rc1->context_type("PARAMETRIC_REPRESENTATION_CONTEXT()");
	rc1->coordinate_space_dimension(2);

	stp_axis2_placement_3d * a2p34 = pnew stp_axis2_placement_3d("", p5, dir1, dir2);

	stp_circle * circ3 = pnew stp_circle;
	circ3->position(a2p3);
	circ3->radius(temp);

	stp_line * l3 = pnew stp_line;
	l3->pnt(p7);
	l3->dir(vec3);

	stp_line * l4 = pnew stp_line;
	l4->pnt(p10);
	l4->dir(vec3);

	stp_line * l5 = pnew stp_line;
	l5->pnt(p10);
	l5->dir(vec2);

	stp_circle * circ4 = pnew stp_circle;
	circ4->position(a2p3);
	circ4->radius(temp);

	//Definitional Representation and plane/vector definitions
	stp_definitional_representation * dr6 = pnew stp_definitional_representation;
	dr6->items()->add(circ4);
	dr6->context_of_items(rc1);

	stp_plane * plane4 = pnew stp_plane;
	plane4->position(a2p32);

	stp_definitional_representation * dr5 = pnew stp_definitional_representation;
	dr5->items()->add(l5);
	dr5->context_of_items(rc1);

	stp_axis2_placement_2d * a2p2d = pnew stp_axis2_placement_2d;
	a2p2d->location(p10);
	a2p2d->ref_direction(dir5);

	stp_axis2_placement * a2p2 = pnew stp_axis2_placement;
	rose_put_nested_object(a2p2, a2p2d);

	stp_definitional_representation * dr4 = pnew stp_definitional_representation;
	dr4->items()->add(l4);
	dr4->context_of_items(rc1);
	
	stp_definitional_representation * dr3 = pnew stp_definitional_representation;
	dr3->items()->add(l3);
	dr3->context_of_items(rc1);
	
	stp_vector * vec1 = pnew stp_vector;
	vec1->orientation(dir1);
	vec1->magnitude(1);
	
	stp_definitional_representation * dr2 = pnew stp_definitional_representation;
	dr2->items()->add(circ3);
	dr2->context_of_items(rc1);
	
	stp_plane * plane3 = pnew stp_plane;
	plane3->position(a2p34);
	
	stp_definitional_representation * dr1 = pnew stp_definitional_representation;
	dr1->items()->add(l2);
	dr1->context_of_items(rc1);
	
	stp_axis2_placement_3d* ap3d = pnew stp_axis2_placement_3d("", p5, dir1, dir2);

	stp_axis2_placement * a2p1 = pnew stp_axis2_placement;
	rose_put_nested_object(a2p1, ap3d);

	//Pcurve and surface geometry definition
	stp_cartesian_point* p3 = pnew stp_cartesian_point;
	p3->name("");
	p3->coordinates()->add(temp);
	p3->coordinates()->add(0);
	p3->coordinates()->add(hei);

	stp_circle * circ1 = pnew stp_circle;
	circ1->position(a2p1);
	circ1->radius(temp);

	stp_pcurve * pc1 = pnew stp_pcurve;
	pc1->basis_surface(cs1);
	pc1->reference_to_curve(dr1);

	stp_pcurve_or_surface * pcos1 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos1, pc1);

	stp_pcurve * pc2 = pnew stp_pcurve;
	pc2->basis_surface(plane3);
	pc2->reference_to_curve(dr2);

	stp_pcurve_or_surface * pcos2 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos2, pc2);

	stp_cartesian_point* p4 = pnew stp_cartesian_point;
	p4->name("");
	p4->coordinates()->add(temp);
	p4->coordinates()->add(0);
	p4->coordinates()->add(0);

	stp_line * l1 = pnew stp_line;
	l1->pnt(p4);
	l1->dir(vec1);

	stp_pcurve * pc3 = pnew stp_pcurve;
	pc3->basis_surface(cs1);
	pc3->reference_to_curve(dr3);

	stp_pcurve_or_surface * pcos3 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos3, pc3);

	stp_pcurve * pc4 = pnew stp_pcurve;
	pc4->basis_surface(cs1);
	pc4->reference_to_curve(dr4);

	stp_pcurve_or_surface * pcos4 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos4, pc4);

	stp_circle * circ2 = pnew stp_circle;
	circ2->position(a2p4);
	circ2->radius(temp);

	stp_pcurve * pc5 = pnew stp_pcurve;
	pc5->basis_surface(cs1);
	pc5->reference_to_curve(dr5);

	stp_pcurve_or_surface * pcos5 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos5, pc5);

	stp_pcurve * pc6 = pnew stp_pcurve;
	pc6->basis_surface(plane4);
	pc6->reference_to_curve(dr6);

	stp_pcurve_or_surface * pcos6 = pnew stp_pcurve_or_surface;
	rose_put_nested_object(pcos6, pc6);

	//Vertex Point and Surface Curve definition
	stp_vertex_point * vp1 = pnew stp_vertex_point;
	vp1->vertex_geometry(p3);

	stp_surface_curve * sc1 = pnew stp_surface_curve;
	sc1->curve_3d(circ1);
	sc1->associated_geometry()->add(pcos1);
	sc1->associated_geometry()->add(pcos2);
	sc1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

	stp_vertex_point * vp2 = pnew stp_vertex_point;
	vp2->vertex_geometry(p4);	

	stp_seam_curve * seam1 = pnew stp_seam_curve;
	seam1->curve_3d(l1);
	seam1->associated_geometry()->add(pcos3);
	seam1->associated_geometry()->add(pcos4);
	seam1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

	stp_surface_curve * sc2 = pnew stp_surface_curve;
	sc2->curve_3d(circ2);
	sc2->associated_geometry()->add(pcos5);
	sc2->associated_geometry()->add(pcos6);
	sc2->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);
	
	//Edge Curve definition
	stp_edge_curve * ec1 = pnew stp_edge_curve;
	ec1->edge_start(vp1);
	ec1->edge_end(vp1);
	ec1->edge_geometry(sc1);
	ec1->same_sense(1);

	stp_edge_curve * ec2 = pnew stp_edge_curve;
	ec2->edge_start(vp2);
	ec2->edge_end(vp1);
	ec2->edge_geometry(seam1);
	ec2->same_sense(1);

	stp_edge_curve * ec3 = pnew stp_edge_curve;
	ec3->edge_start(vp2);
	ec3->edge_end(vp2);
	ec3->edge_geometry(sc2);
	ec3->same_sense(1);


	//Oriented Edge Definitions
	stp_oriented_edge * oe1 = pnew stp_oriented_edge;
	oe1->edge_element(ec1);
	oe1->orientation(0);

	stp_oriented_edge * oe2 = pnew stp_oriented_edge;
	oe2->edge_element(ec2);
	oe2->orientation(0);

	stp_oriented_edge * oe3 = pnew stp_oriented_edge;
	oe3->edge_element(ec3);
	oe3->orientation(1);

	stp_oriented_edge * oe4 = pnew stp_oriented_edge;
	oe4->edge_element(ec2);
	oe4->orientation(1);

	stp_oriented_edge * oe5 = pnew stp_oriented_edge;
	oe5->edge_element(ec1);
	oe5->orientation(1);

	stp_oriented_edge * oe6 = pnew stp_oriented_edge;
	oe6->edge_element(ec3);
	oe6->orientation(0);

	//Edge Loop definition and Axis2 Placements
	stp_edge_loop * el1 = pnew stp_edge_loop;
	el1->edge_list()->add(oe1);
	el1->edge_list()->add(oe2);
	el1->edge_list()->add(oe3);
	el1->edge_list()->add(oe4);
	
	stp_edge_loop * el2 = pnew stp_edge_loop;
	el2->edge_list()->add(oe5);

	stp_cartesian_point* p2 = pnew stp_cartesian_point;
	p2->name("");
	p2->coordinates()->add(0);
	p2->coordinates()->add(0);
	p2->coordinates()->add(hei);

	stp_axis2_placement_3d * a2p33 = pnew stp_axis2_placement_3d("", p2, dir1, dir2);

	stp_edge_loop * el3 = pnew stp_edge_loop;
	el3->edge_list()->add(oe6);


	//Face bounds and surface declarations
	stp_face_bound * b1 = pnew stp_face_bound;
	b1->bound(el1);
	b1->orientation(1);

	stp_face_bound * b2 = pnew stp_face_bound;
	b2->bound(el2);
	b2->orientation(1);

	stp_plane * plane1 = pnew stp_plane;
	plane1->position(a2p33);

	stp_face_bound * b3 = pnew stp_face_bound;
	b3->bound(el3);
	b3->orientation(1);

	stp_plane * plane2 = pnew stp_plane;
	plane2->position(a2p32);
	
	//Advanced Faces declarations
	stp_advanced_face * f1 = pnew stp_advanced_face;
	f1->bounds()->add(b1);
	f1->face_geometry(cs1);
	f1->same_sense(1);

	stp_advanced_face * f2 = pnew stp_advanced_face;
	f2->bounds()->add(b2);
	f2->face_geometry(plane1);
	f2->same_sense(1);

	stp_advanced_face * f3 = pnew stp_advanced_face;
	f3->bounds()->add(b3);
	f3->face_geometry(plane2);
	f3->same_sense(0);

	//Closed Shell Declaration
	stp_closed_shell * sh = pnew stp_closed_shell;
	sh->cfs_faces()->add(f1);
	sh->cfs_faces()->add(f2);
	sh->cfs_faces()->add(f3);
	
	//Manifold solid brep and axis2_placement_3d 
	stp_manifold_solid_brep * brep = pnew stp_manifold_solid_brep;
	brep->outer(sh);


	stp_axis2_placement_3d* a2p5 = pnew stp_axis2_placement_3d("", p1, dir1, dir2);

	advsol->items()->add(a2p5);
	advsol->items()->add(brep);

	return;
}