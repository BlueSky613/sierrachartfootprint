import express from 'express';
import {controller} from '../../controllers/controller.js';

const routes = express.Router();

routes.post("/footprint", controller.postFootPrint);

export {routes}