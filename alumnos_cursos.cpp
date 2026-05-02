#include "alumnos_cursos.h"
#include "conexion.h"
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Funcion: menuAlumnosCursos()
// Muestra el submenu de asignacion de cursos.
// =============================================
void menuAlumnosCursos()
{
  int opcion;

  do
  {
    cout << "\n===== MODULO ALUMNOS - CURSOS =====" << endl;
    cout << "1. Listar asignaciones" << endl;
    cout << "2. Asignar curso a alumno" << endl;
    cout << "3. Eliminar asignacion" << endl;
    cout << "4. Regresar" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    switch (opcion)
    {
    case 1:
      listarAlumnosCursos();
      break;
    case 2:
      asignarCursoAlumno();
      break;
    case 3:
      eliminarAsignacionCurso();
      break;
    case 4:
      cout << "Regresando al menu principal..." << endl;
      break;
    default:
      cout << "Opcion invalida. Intente de nuevo." << endl;
    }

  } while (opcion != 4);
}

// =============================================
// Funcion: listarAlumnosCursos()
// Muestra todas las asignaciones de cursos
// a alumnos con sus nombres completos.
// =============================================
void listarAlumnosCursos()
{
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  const char *sql =
      "SELECT ac.id, a.nombres, a.apellidos, c.nombre AS curso, c.codigo "
      "FROM alumnos_cursos ac "
      "JOIN alumnos a ON ac.alumno_id = a.id "
      "JOIN cursos c  ON ac.curso_id  = c.id "
      "ORDER BY a.apellidos, a.nombres";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql, SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo ejecutar la consulta." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
  }

  SQLINTEGER id;
  SQLCHAR nombres[100], apellidos[100], curso[100], codigo[20];
  SQLLEN len;

  cout << "\n--- ASIGNACIONES ALUMNO - CURSO ---" << endl;
  cout << "------------------------------------------------------------" << endl;

  bool hayDatos = false;

  while (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    hayDatos = true;
    SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &len);
    SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, 100, &len);
    SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, 100, &len);
    SQLGetData(hStmt, 4, SQL_C_CHAR, curso, 100, &len);
    SQLGetData(hStmt, 5, SQL_C_CHAR, codigo, 20, &len);

    cout << "AsignID: " << id
         << " | Alumno: " << nombres << " " << apellidos
         << " | Curso: " << curso
         << " (" << codigo << ")" << endl;
  }

  if (!hayDatos)
  {
    cout << "No hay asignaciones registradas." << endl;
  }

  cout << "------------------------------------------------------------" << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: asignarCursoAlumno()
// Asigna un curso a un alumno.
// Valida que no se asigne el mismo curso
// dos veces al mismo alumno.
// =============================================
void asignarCursoAlumno()
{
  int alumnoId, cursoId;

  cout << "\n--- ASIGNAR CURSO A ALUMNO ---" << endl;
  cout << "ID del alumno : ";
  cin >> alumnoId;
  cout << "ID del curso  : ";
  cin >> cursoId;

  // Verificar si la asignacion ya existe
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sqlVerificar = "SELECT COUNT(*) FROM alumnos_cursos "
                        "WHERE alumno_id=" +
                        to_string(alumnoId) +
                        " AND curso_id=" + to_string(cursoId);

  SQLExecDirect(hStmt, (SQLCHAR *)sqlVerificar.c_str(), SQL_NTS);

  SQLINTEGER existe = 0;
  SQLLEN len;
  if (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    SQLGetData(hStmt, 1, SQL_C_LONG, &existe, 0, &len);
  }
  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

  if (existe > 0)
  {
    cout << "[AVISO] Este alumno ya tiene ese curso asignado." << endl;
    return;
  }

  // Insertar la asignacion
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES (" + to_string(alumnoId) + ", " + to_string(cursoId) + ")";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Curso asignado al alumno correctamente." << endl;
  else
    cout << "[ERROR] No se pudo realizar la asignacion. Verifica los IDs." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: eliminarAsignacionCurso()
// Elimina una asignacion alumno-curso
// usando el ID de la asignacion.
// =============================================
void eliminarAsignacionCurso()
{
  int id;

  cout << "\n--- ELIMINAR ASIGNACION CURSO ---" << endl;
  cout << "ID de la asignacion a eliminar: ";
  cin >> id;

  char confirmar;
  cout << "Esta seguro? (s/n): ";
  cin >> confirmar;

  if (confirmar != 's' && confirmar != 'S')
  {
    cout << "Operacion cancelada." << endl;
    return;
  }

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "DELETE FROM alumnos_cursos WHERE id=" + to_string(id);
  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Asignacion eliminada correctamente." << endl;
  else
    cout << "[ERROR] No se pudo eliminar la asignacion." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}