-- =============================================
-- Script SQL: Base de datos UNIVERSIDAD
-- Motor: PostgreSQL
-- =============================================

-- Crear la base de datos (ejecutar esto separado si no existe)
-- CREATE DATABASE universidad;

-- Conectarse a la base de datos antes de ejecutar lo siguiente:
-- \c universidad

-- =============================================
-- TABLA: alumnos
-- =============================================
CREATE TABLE IF NOT EXISTS alumnos (
    id SERIAL PRIMARY KEY,
    nombres VARCHAR(100) NOT NULL,
    apellidos VARCHAR(100) NOT NULL,
    carnet VARCHAR(20) NOT NULL
);

-- =============================================
-- TABLA: cursos
-- =============================================
CREATE TABLE IF NOT EXISTS cursos (
    id SERIAL PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    codigo VARCHAR(20) NOT NULL
);

-- =============================================
-- TABLA: secciones
-- =============================================
CREATE TABLE IF NOT EXISTS secciones (
    id SERIAL PRIMARY KEY,
    nombre VARCHAR(50) NOT NULL,
    jornada VARCHAR(50) NOT NULL
);

-- =============================================
-- TABLA: alumnos_cursos (relacion muchos a muchos)
-- Un alumno puede tener varios cursos,
-- pero no puede repetir el mismo curso.
-- =============================================
CREATE TABLE IF NOT EXISTS alumnos_cursos (
    id SERIAL PRIMARY KEY,
    alumno_id INT NOT NULL REFERENCES alumnos (id) ON DELETE CASCADE,
    curso_id INT NOT NULL REFERENCES cursos (id) ON DELETE CASCADE,
    UNIQUE (alumno_id, curso_id)
);

-- =============================================
-- TABLA: alumnos_seccion (relacion uno a uno)
-- Cada alumno puede tener como maximo una seccion.
-- =============================================
CREATE TABLE IF NOT EXISTS alumnos_seccion (
    id SERIAL PRIMARY KEY,
    alumno_id INT NOT NULL REFERENCES alumnos (id) ON DELETE CASCADE UNIQUE,
    seccion_id INT NOT NULL REFERENCES secciones (id) ON DELETE CASCADE
);