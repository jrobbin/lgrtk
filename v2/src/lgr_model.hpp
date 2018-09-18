#ifndef LGR_MODEL_HPP
#define LGR_MODEL_HPP

#include <lgr_field_index.hpp>
#include <lgr_field_access.hpp>
#include <Omega_h_teuchos.hpp>
#include <lgr_element_types.hpp>
#include <lgr_remap_type.hpp>
#include <lgr_class_names.hpp>

namespace lgr {

struct Simulation;
struct Support;

enum ModelOrder {
  IS_FIELD_UPDATE,
  AFTER_FIELD_UPDATE,
  BEFORE_MATERIAL_MODEL,
  IS_MATERIAL_MODEL,
  AFTER_MATERIAL_MODEL,
  BEFORE_FORCES,
};

struct ModelBase {
  Simulation& sim;
  Support* elem_support;
  Support* point_support;
  virtual ~ModelBase() = default;
  virtual void out_of_line_virtual_function();
  ModelBase(Simulation& sim_in, ClassNames const& class_names);
  ModelBase(Simulation& sim_in, Teuchos::ParameterList& pl);
  virtual ModelOrder order() = 0;
  virtual char const* name() = 0;
  virtual void update_state() = 0;
  FieldIndex point_define(std::string const& short_name, std::string const& long_name,
      int ncomps);
  FieldIndex point_define(std::string const& short_name, std::string const& long_name,
      int ncomps, std::string const& default_value);
  FieldIndex point_define(std::string const& short_name, std::string const& long_name,
      int ncomps, RemapType tt, std::string const& default_value);
  FieldIndex elem_define(std::string const& short_name, std::string const& long_name,
      int ncomps);
  FieldIndex elem_define(std::string const& short_name, std::string const& long_name,
      int ncomps, std::string const& default_value);
  FieldIndex elem_define(std::string const& short_name, std::string const& long_name,
      int ncomps, RemapType tt, std::string const& default_value);
  MappedElemsToNodes get_elems_to_nodes();
  int points();
  MappedRead elems_get(FieldIndex fi);
  MappedWrite elems_set(FieldIndex fi);
  MappedWrite elems_getset(FieldIndex fi);
};

template <class Elem>
struct Model : public ModelBase {
  Model(Simulation&, Teuchos::ParameterList&);
  Model(Simulation&, ClassNames const&);
  MappedPointRead<Elem> points_get(FieldIndex fi);
  MappedPointWrite<Elem> points_set(FieldIndex fi);
  MappedPointWrite<Elem> points_getset(FieldIndex fi);
  MappedPointRead<Elem> elems_get(FieldIndex fi);
  MappedPointWrite<Elem> elems_set(FieldIndex fi);
  MappedPointWrite<Elem> elems_getset(FieldIndex fi);
};

#define LGR_EXPL_INST(Elem) \
extern template struct Model<Elem>;
LGR_EXPL_INST_ELEMS
#undef LGR_EXPL_INST

}

#endif