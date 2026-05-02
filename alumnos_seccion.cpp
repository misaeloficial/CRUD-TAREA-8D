#include "alumnos_seccion.h"
#include "conexion.h"
#include <iostream>
#include <string>

using namespace std;

// =============================================
// Funcion: menuAlumnosSeccion()
// Muestra el submenu de asignacion de seccion.
// =============================================
void menuAlumnosSeccion()
{
  int opcion;

  do
  {
    cout << "\n===== MODULO ALUMNOS - SECCION =====" << endl;
    cout << "1. Listar asignaciones" << endl;
    cout << "2. Asignar seccion a alumno" << endl;
    cout << "3. Modificar seccion de alumno" << endl;
    cout << "4. Eliminar asignacion" << endl;
    cout << "5. Regresar" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    switch (opcion)
    {
    case 1:
      listarAlumnosSeccion();
      break;
    case 2:
      asignarSeccionAlumno();
      break;
    case 3:
      modificarSeccionAlumno();
      break;
    case 4:
      eliminarAsignacionSeccion();
      break;
    case 5:
      cout << "Regresando al menu principal..." << endl;
      break;
    default:
      cout << "Opcion invalida. Intente de nuevo." << endl;
    }

  } while (opcion != 5);
}

// =============================================
// Funcion: listarAlumnosSeccion()
// Muestra todas las asignaciones de seccion.
// =============================================
void listarAlumnosSeccion()
{
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  const char *sql =
      "SELECT ase.id, a.nombres, a.apellidos, s.nombre AS seccion, s.jornada "
      "FROM alumnos_seccion ase "
      "JOIN alumnos   a ON ase.alumno_id   = a.id "
      "JOIN secciones s ON ase.seccion_id  = s.id "
      "ORDER BY a.apellidos, a.nombres";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql, SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo ejecutar la consulta." << endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
  }

  SQLINTEGER id;
  SQLCHAR nombres[100], apellidos[100], seccion[50], jornada[50];
  SQLLEN len;

  cout << "\n--- ASIGNACIONES ALUMNO - SECCION ---" << endl;
  cout << "------------------------------------------------------------" << endl;

  bool hayDatos = false;

  while (SQLFetch(hStmt) == SQL_SUCCESS)
  {
    hayDatos = true;
    SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &len);
    SQLGetData(hStmt, 2, SQL_C_CHAR, nombres, 100, &len);
    SQLGetData(hStmt, 3, SQL_C_CHAR, apellidos, 100, &len);
    SQLGetData(hStmt, 4, SQL_C_CHAR, seccion, 50, &len);
    SQLGetData(hStmt, 5, SQL_C_CHAR, jornada, 50, &len);

    cout << "AsignID: " << id
         << " | Alumno: " << nombres << " " << apellidos
         << " | Seccion: " << seccion
         << " | Jornada: " << jornada << endl;
  }

  if (!hayDatos)
  {
    cout << "No hay asignaciones de seccion registradas." << endl;
  }

  cout << "------------------------------------------------------------" << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: asignarSeccionAlumno()
// Asigna una seccion a un alumno.
// Si ya tiene una seccion, informa al usuario
// que debe usar la opcion Modificar.
// =============================================
void asignarSeccionAlumno()
{
  int alumnoId, seccionId;

  cout << "\n--- ASIGNAR SECCION A ALUMNO ---" << endl;
  cout << "ID del alumno  : ";
  cin >> alumnoId;
  cout << "ID de la seccion: ";
  cin >> seccionId;

  // Verificar si el alumno ya tiene seccion asignada
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sqlVerificar = "SELECT COUNT(*) FROM alumnos_seccion WHERE alumno_id=" + to_string(alumnoId);
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
    cout << "[AVISO] Este alumno ya tiene una seccion asignada." << endl;
    cout << "Use la opcion 3 (Modificar) para cambiarla." << endl;
    return;
  }

  // Insertar la asignacion
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "INSERT INTO alumnos_seccion (alumno_id, seccion_id) VALUES (" + to_string(alumnoId) + ", " + to_string(seccionId) + ")";

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Seccion asignada al alumno correctamente." << endl;
  else
    cout << "[ERROR] No se pudo realizar la asignacion. Verifica los IDs." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: modificarSeccionAlumno()
// Cambia la seccion asignada a un alumno.
// =============================================
void modificarSeccionAlumno()
{
  int alumnoId, nuevaSeccionId;

  cout << "\n--- MODIFICAR SECCION DE ALUMNO ---" << endl;
  cout << "ID del alumno           : ";
  cin >> alumnoId;
  cout << "ID de la nueva seccion  : ";
  cin >> nuevaSeccionId;

  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  string sql = "UPDATE alumnos_seccion SET seccion_id=" + to_string(nuevaSeccionId) +
               " WHERE alumno_id=" + to_string(alumnoId);

  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Seccion del alumno actualizada correctamente." << endl;
  else
    cout << "[ERROR] No se pudo actualizar. Verifica los IDs." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

// =============================================
// Funcion: eliminarAsignacionSeccion()
// Elimina la asignacion de seccion de un alumno
// usando el ID de la asignacion.
// =============================================
void eliminarAsignacionSeccion()
{
  int id;

  cout << "\n--- ELIMINAR ASIGNACION SECCION ---" << endl;
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

  string sql = "DELETE FROM alumnos_seccion WHERE id=" + to_string(id);
  SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    cout << "[OK] Asignacion de seccion eliminada correctamente." << endl;
  else
    cout << "[ERROR] No se pudo eliminar la asignacion." << endl;

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}