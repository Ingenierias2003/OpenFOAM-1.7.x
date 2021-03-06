/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
    
\*---------------------------------------------------------------------------*/

#include "steadyStateDdtScheme.H"
#include "fvcDiv.H"
#include "fvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateDdtScheme<Type>::fvcDdt
(
    const dimensioned<Type>& dt
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "ddt("+dt.name()+')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                dt.dimensions()/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateDdtScheme<Type>::fvcDdt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "ddt("+vf.name()+')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                vf.dimensions()/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateDdtScheme<Type>::fvcDdt
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "ddt("+rho.name()+','+vf.name()+')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                rho.dimensions()*vf.dimensions()/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateDdtScheme<Type>::fvcDdt
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "ddt("+rho.name()+','+vf.name()+')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<Type>
            (
                "0",
                rho.dimensions()*vf.dimensions()/dimTime,
                pTraits<Type>::zero
            )
        )
    );
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateDdtScheme<Type>::fvmDdt
(
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            vf.dimensions()*dimVol/dimTime
        )
    );

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateDdtScheme<Type>::fvmDdt
(
    const dimensionedScalar& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type> >
steadyStateDdtScheme<Type>::fvmDdt
(
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );

    return tfvm;
}


template<class Type>
tmp<typename steadyStateDdtScheme<Type>::fluxFieldType>
steadyStateDdtScheme<Type>::fvcDdtPhiCorr
(
    const volScalarField& rA,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtPhiCorr("
              + rA.name() + ',' + U.name() + ',' + phi.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<typename flux<Type>::type>
            (
                "0",
                rA.dimensions()*phi.dimensions()/dimTime,
                pTraits<typename flux<Type>::type>::zero
            )
        )
    );
}


template<class Type>
tmp<typename steadyStateDdtScheme<Type>::fluxFieldType>
steadyStateDdtScheme<Type>::fvcDdtPhiCorr
(
    const volScalarField& rA,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const fluxFieldType& phi
)
{
    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtPhiCorr("
              + rA.name() + ',' + rho.name()
              + ',' + U.name() + ',' + phi.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensioned<typename flux<Type>::type>
            (
                "0",
                rA.dimensions()*rho.dimensions()*phi.dimensions()/dimTime,
                pTraits<typename flux<Type>::type>::zero
            )
        )
    );
}


template<class Type>
tmp<surfaceScalarField> steadyStateDdtScheme<Type>::meshPhi
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            IOobject
            (
                "meshPhi",
                mesh().time().timeName(),
                mesh()
            ),
            mesh(),
            dimensionedScalar("0", dimVolume/dimTime, 0.0)
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
